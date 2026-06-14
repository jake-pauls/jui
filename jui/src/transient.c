#include "jui/transient.h"

#include "jui/core.h"
#include "jui/core_memory.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <assert.h>

#define MAXIMUM_WINDOW_COUNT (3)
#define MAXIMUM_DRAW_LIST_SIZE_PER_FRAME (100)
#define MAXIMUM_FRAME_ARENA_SIZE_BYTES (1024)

#define DEFAULT_WIDGET_ID (0)
#define UNINITIALIZED_FRAME_POSITION (-1)

/* Forward Declarations */

typedef void (*jui_draw_command_fn)(const void*);
typedef struct jui_draw_list jui_draw_list;
typedef struct sdl_draw_rect_params sdl_draw_rect_params;
typedef struct sdl_draw_text_params sdl_draw_text_params;

typedef struct jui_rect jui_rect;
typedef struct jui_window jui_window;

typedef struct jui_state jui_state;
typedef struct jui_sdl_state jui_sdl_state;

/* Rendering */

struct jui_draw_list
{
  jui_draw_command_fn Command[MAXIMUM_DRAW_LIST_SIZE_PER_FRAME];
  void* Parameters[MAXIMUM_DRAW_LIST_SIZE_PER_FRAME];
  u32 Count;
};

struct sdl_draw_rect_params
{
  jui_rect* Rect;
  u8 R, G, B, A;
};

struct sdl_draw_text_params
{
  const char* Text;
  f32 X, Y;
};

/* UI Structures */

/// \brief Represents a rectangle using floating point values
struct jui_rect
{
  f32 X, Y, W, H;
};

/// \brief Represents a window widget in jui. Contains a draw list of command
///        for rendering each widget in the window that are executed at the end
///        of the frame.
struct jui_window
{
  jui_id Id;
  f32 X, Y;
  jui_draw_list DrawList;
};

/* State */

struct jui_state
{
  f32 MouseX;
  f32 MouseY;
  f32 DragOffsetX;
  f32 DragOffsetY;

  bool MouseDown;
  bool MousePressed;
  bool MouseReleased;

  arena FrameArena;

  // TODO(jake): Abstract this into a different data structure (e.g., a list or map with positions per frame).
  //             I think that having frames be nested (in effect, layouts) is a much more idiomatic approach.
  //             If we have bespoke frames finitely containing widgets, or, `begin`/`end` calls, we make
  //             way for frames to be something other than just another bespoke widget. If we just generally
  //             'layout' every widget and make the frame/window another widget with other widgets laid out
  //             inside it, the architecture might be much cleaner.
  //
  // TODO(jake): Likely this WindowId just look at another data structure (e.g., a stack) - or just maintain a pointer
  //             to the current window in the state directly.
  jui_window* CurrentWindow;
  jui_window Windows[MAXIMUM_WINDOW_COUNT];

  jui_id HotItem;
  jui_id ActiveItem;
};

struct jui_sdl_state
{
  SDL_Renderer* Renderer;
};

static jui_state State = {
    .MouseX = 0,
    .MouseY = 0,
    .DragOffsetX = 0,
    .DragOffsetY = 0,
    .MouseDown = false,
    .MousePressed = false,
    .MouseReleased = false,
    .FrameArena =
        {
            .Base = NULL,
            .Position = 0,
        },
    .CurrentWindow = NULL,
    // TODO(jake): Find a way to "semi-dynamically" add windows
    .Windows =
        {
            {
                .Id = DEFAULT_WIDGET_ID,
                .X = UNINITIALIZED_FRAME_POSITION,
                .Y = UNINITIALIZED_FRAME_POSITION,
                .DrawList =
                    {
                        .Command = {0},
                        .Parameters = {0},
                        .Count = 0,
                    },
            },
            {
                .Id = DEFAULT_WIDGET_ID,
                .X = UNINITIALIZED_FRAME_POSITION,
                .Y = UNINITIALIZED_FRAME_POSITION,
                .DrawList =
                    {
                        .Command = {0},
                        .Parameters = {0},
                        .Count = 0,
                    },
            },
            {
                .Id = DEFAULT_WIDGET_ID,
                .X = UNINITIALIZED_FRAME_POSITION,
                .Y = UNINITIALIZED_FRAME_POSITION,
                .DrawList =
                    {
                        .Command = {0},
                        .Parameters = {0},
                        .Count = 0,
                    },
            },
        },
    .HotItem = DEFAULT_WIDGET_ID,
    .ActiveItem = DEFAULT_WIDGET_ID,
};

static jui_sdl_state SDLState = {
    .Renderer = NULL,
};

/* Platform (SDL) */

void jui_sdl_consume_event(const SDL_Event* Event)
{
  // Reset the mouse pressed/released state every time an event is consumed.
  State.MousePressed = false;
  State.MouseReleased = false;

  switch (Event->type)
  {
  case SDL_EVENT_MOUSE_MOTION:
    State.MouseX = Event->motion.x;
    State.MouseY = Event->motion.y;
    break;
  case SDL_EVENT_MOUSE_BUTTON_DOWN:
    if (Event->button.button == SDL_BUTTON_LEFT)
    {
      State.MouseDown = true;
      State.MousePressed = true;
    }
    break;
  case SDL_EVENT_MOUSE_BUTTON_UP:
    if (Event->button.button == SDL_BUTTON_LEFT)
    {
      if (State.MouseDown)
      {
        State.MouseReleased = true;
      }
      State.MouseDown = false;
    }
    break;
  default:;
  }
}

/* Lifecycle (General) */

void jui_init()
{
  // Allocate the frame-by-frame memory arena.
  arena_alloc(&State.FrameArena, MAXIMUM_FRAME_ARENA_SIZE_BYTES);
}

void jui_start_frame()
{
  // At the start of each frame clear the frame-by-frame memory arena.
  arena_clear(&State.FrameArena);
}

void jui_render()
{
  for (size_t WindowIdx = 0; WindowIdx < MAXIMUM_WINDOW_COUNT; ++WindowIdx)
  {
    // Execute each command in the draw list for each window.
    for (size_t Idx = 0; Idx < State.Windows[WindowIdx].DrawList.Count; ++Idx)
    {
      jui_draw_list* DrawList = &State.Windows[WindowIdx].DrawList;
      jui_draw_command_fn Command = DrawList->Command[Idx];
      void* Parameters = DrawList->Parameters[Idx];
      Command(Parameters);
    }

    // Reset the window draw list for each window.
    State.Windows[WindowIdx].DrawList.Count = 0;
  }
}

/* Renderer (SDL) */

void jui_sdl_init(SDL_Renderer* Renderer)
{
  SDLState.Renderer = Renderer;

  // NOTE(jake): Adding this in the backend init call for now because I don't think the user should have to care about calling it.
  jui_init();
}

void sdl_draw_rect_impl(SDL_Renderer* Renderer, const jui_rect* const Rect, const u8 R, const u8 G, const u8 B, const u8 A)
{
  SDL_FRect FRect;
  FRect.x = Rect->X;
  FRect.y = Rect->Y;
  FRect.w = Rect->W;
  FRect.h = Rect->H;
  SDL_SetRenderDrawColor(Renderer, R, G, B, A);
  SDL_RenderFillRect(Renderer, &FRect);
}

void sdl_draw_rect(const void* Params)
{
  const sdl_draw_rect_params* DrawRectParams = Params;
  sdl_draw_rect_impl(SDLState.Renderer, DrawRectParams->Rect, DrawRectParams->R, DrawRectParams->G, DrawRectParams->B, DrawRectParams->A);
}

void sdl_draw_text_impl(SDL_Renderer* Renderer, const char* Text, const f32 X, const f32 Y)
{
  SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
  SDL_RenderDebugText(Renderer, X, Y, Text);
}

void sdl_draw_text(const void* Params)
{
  const sdl_draw_text_params* DrawTextParams = Params;
  sdl_draw_text_impl(SDLState.Renderer, DrawTextParams->Text, DrawTextParams->X, DrawTextParams->Y);
}

void jui_sdl_draw_frame(const jui_id Id, const char* Text, const jui_rect* const Rect)
{
  if (State.ActiveItem == Id)
  {
    // Active color (95% pink)
    sdl_draw_rect_params* DrawRectParams = PUSH_STRUCT(&State.FrameArena, sdl_draw_rect_params);

    jui_rect* DrawRect = PUSH_STRUCT(&State.FrameArena, jui_rect);
    DrawRect->X = Rect->X;
    DrawRect->Y = Rect->Y;
    DrawRect->W = Rect->W;
    DrawRect->H = Rect->H;

    DrawRectParams->Rect = DrawRect;
    DrawRectParams->R = 255;
    DrawRectParams->G = 230;
    DrawRectParams->B = 242;
    DrawRectParams->A = 255;

    State.CurrentWindow->DrawList.Command[State.CurrentWindow->DrawList.Count] = &sdl_draw_rect;
    State.CurrentWindow->DrawList.Parameters[State.CurrentWindow->DrawList.Count] = DrawRectParams;
    State.CurrentWindow->DrawList.Count++;
  }
  else if (State.HotItem == Id)
  {
    // Hot/hover color (90% pink)
    sdl_draw_rect_params* DrawRectParams = PUSH_STRUCT(&State.FrameArena, sdl_draw_rect_params);

    jui_rect* DrawRect = PUSH_STRUCT(&State.FrameArena, jui_rect);
    DrawRect->X = Rect->X;
    DrawRect->Y = Rect->Y;
    DrawRect->W = Rect->W;
    DrawRect->H = Rect->H;

    DrawRectParams->Rect = DrawRect;
    DrawRectParams->R = 255;
    DrawRectParams->G = 204;
    DrawRectParams->B = 229;
    DrawRectParams->A = 255;

    State.CurrentWindow->DrawList.Command[State.CurrentWindow->DrawList.Count] = &sdl_draw_rect;
    State.CurrentWindow->DrawList.Parameters[State.CurrentWindow->DrawList.Count] = DrawRectParams;
    State.CurrentWindow->DrawList.Count++;
  }
  else
  {
    // Inactive (80% pink)
    sdl_draw_rect_params* DrawRectParams = PUSH_STRUCT(&State.FrameArena, sdl_draw_rect_params);

    jui_rect* DrawRect = PUSH_STRUCT(&State.FrameArena, jui_rect);
    DrawRect->X = Rect->X;
    DrawRect->Y = Rect->Y;
    DrawRect->W = Rect->W;
    DrawRect->H = Rect->H;

    DrawRectParams->Rect = DrawRect;
    DrawRectParams->R = 255;
    DrawRectParams->G = 153;
    DrawRectParams->B = 202;
    DrawRectParams->A = 255;

    State.CurrentWindow->DrawList.Command[State.CurrentWindow->DrawList.Count] = &sdl_draw_rect;
    State.CurrentWindow->DrawList.Parameters[State.CurrentWindow->DrawList.Count] = DrawRectParams;
    State.CurrentWindow->DrawList.Count++;
  }

  // TODO(jake): Proper layouting, right now just add some arbitrary margin around the frame text.
  sdl_draw_text_params* DrawTextParams = PUSH_STRUCT(&State.FrameArena, sdl_draw_text_params);

  // TODO(jake): Need to make some string abstractions
  char* AllocatedText = PUSH_ARRAY(&State.FrameArena, char, strlen(Text) + 1);
  strcpy(&AllocatedText[0], &Text[0]);

  DrawTextParams->Text = AllocatedText;
  DrawTextParams->X = Rect->X + 2;
  DrawTextParams->Y = Rect->Y + 2;

  State.CurrentWindow->DrawList.Command[State.CurrentWindow->DrawList.Count] = &sdl_draw_text;
  State.CurrentWindow->DrawList.Parameters[State.CurrentWindow->DrawList.Count] = DrawTextParams;
  State.CurrentWindow->DrawList.Count++;
}

void jui_sdl_draw_button(const jui_id Id, const char* Text, const jui_rect* const Rect)
{
  if (State.ActiveItem == Id)
  {
    // Active color (green)
    sdl_draw_rect_params* DrawRectParams = PUSH_STRUCT(&State.FrameArena, sdl_draw_rect_params);

    jui_rect* DrawRect = PUSH_STRUCT(&State.FrameArena, jui_rect);
    DrawRect->X = Rect->X;
    DrawRect->Y = Rect->Y;
    DrawRect->W = Rect->W;
    DrawRect->H = Rect->H;

    DrawRectParams->Rect = DrawRect;
    DrawRectParams->R = 0;
    DrawRectParams->G = 255;
    DrawRectParams->B = 0;
    DrawRectParams->A = 255;

    State.CurrentWindow->DrawList.Command[State.CurrentWindow->DrawList.Count] = &sdl_draw_rect;
    State.CurrentWindow->DrawList.Parameters[State.CurrentWindow->DrawList.Count] = DrawRectParams;
    State.CurrentWindow->DrawList.Count++;
  }
  else if (State.HotItem == Id)
  {
    // Hot/hover color (cyan)
    sdl_draw_rect_params* DrawRectParams = PUSH_STRUCT(&State.FrameArena, sdl_draw_rect_params);

    jui_rect* DrawRect = PUSH_STRUCT(&State.FrameArena, jui_rect);
    DrawRect->X = Rect->X;
    DrawRect->Y = Rect->Y;
    DrawRect->W = Rect->W;
    DrawRect->H = Rect->H;

    DrawRectParams->Rect = DrawRect;
    DrawRectParams->R = 0;
    DrawRectParams->G = 255;
    DrawRectParams->B = 255;
    DrawRectParams->A = 255;

    State.CurrentWindow->DrawList.Command[State.CurrentWindow->DrawList.Count] = &sdl_draw_rect;
    State.CurrentWindow->DrawList.Parameters[State.CurrentWindow->DrawList.Count] = DrawRectParams;
    State.CurrentWindow->DrawList.Count++;
  }
  else
  {
    // Inactive (blue)
    sdl_draw_rect_params* DrawRectParams = PUSH_STRUCT(&State.FrameArena, sdl_draw_rect_params);

    jui_rect* DrawRect = PUSH_STRUCT(&State.FrameArena, jui_rect);
    DrawRect->X = Rect->X;
    DrawRect->Y = Rect->Y;
    DrawRect->W = Rect->W;
    DrawRect->H = Rect->H;

    DrawRectParams->Rect = DrawRect;
    DrawRectParams->R = 0;
    DrawRectParams->G = 0;
    DrawRectParams->B = 255;
    DrawRectParams->A = 255;

    State.CurrentWindow->DrawList.Command[State.CurrentWindow->DrawList.Count] = &sdl_draw_rect;
    State.CurrentWindow->DrawList.Parameters[State.CurrentWindow->DrawList.Count] = DrawRectParams;
    State.CurrentWindow->DrawList.Count++;
  }

  // TODO(jake): Proper layouting, right now it's just an arbitrary position near the center of the button.
  //             Note that the X coordinate is really all that's needed now because text is anchored on the
  //             top right point of the texture.
  sdl_draw_text_params* DrawTextParams = PUSH_STRUCT(&State.FrameArena, sdl_draw_text_params);

  // TODO(jake): Need to make some string abstractions
  char* AllocatedText = PUSH_ARRAY(&State.FrameArena, char, strlen(Text) + 1);
  strcpy(&AllocatedText[0], &Text[0]);

  DrawTextParams->Text = AllocatedText;
  DrawTextParams->X = Rect->X;
  DrawTextParams->Y = Rect->H / 2.0f + Rect->Y;

  State.CurrentWindow->DrawList.Command[State.CurrentWindow->DrawList.Count] = &sdl_draw_text;
  State.CurrentWindow->DrawList.Parameters[State.CurrentWindow->DrawList.Count] = DrawTextParams;
  State.CurrentWindow->DrawList.Count++;
}

/* API */

/// \brief Simple hit test function for a box defined with an \p X and \p Y coordinate as well
///        as a provided width and height that is performed against the global state.
/// \param X The x coordinate used for the hit test.
/// \param Y The y coordinate used for the hit test.
/// \param W The width used for the hit test.
/// \param H The height used for the hit test.
/// \returns <c>true</c> if the hit test passed for the global state, otherwise <c>false</c>.
bool hit_test(const f32 X, const f32 Y, const f32 W, const f32 H)
{
  return !(State.MouseX < X || State.MouseY < Y || State.MouseX >= X + W || State.MouseY >= Y + H);
}

/// \brief Performs a hit test and updates the internal state depending on the result.
/// \param Id The unique identifier of the widget that the hit is being tested against.
/// \param X The x coordinate used for the hit test.
/// \param Y The y coordinate used for the hit test.
/// \param W The width used for the hit test.
/// \param H The height used for the hit test.
bool perform_hit_test(const jui_id Id, const f32 X, const f32 Y, const f32 W, const f32 H)
{
  // Only set an item as hot if we pass the hit test and do not have a currently active item.
  // Useful for preventing a click event on another widget if the user clicks and drags off the button.
  if (hit_test(X, Y, W, H))
  {
    State.HotItem = Id;
    return true;
  }

  // If we fail the hit test and the current item is hot, reset it.
  if (State.HotItem == Id)
  {
    State.HotItem = DEFAULT_WIDGET_ID;
  }

  return false;
}

/* Widgets */

void jui_make_window(jui_window* Window, const char* Text, const f32 W, const f32 H)
{
  // TODO(jake): Remove this when we figure out layouting.
  Window->X = Window->X == UNINITIALIZED_FRAME_POSITION ? 0 : Window->X;
  Window->Y = Window->Y == UNINITIALIZED_FRAME_POSITION ? 0 : Window->Y;

  Window->Id = hash_djb2((uchar*)Text);
  State.CurrentWindow = Window;

  perform_hit_test(Window->Id, Window->X, Window->Y, W, H);

  if (State.HotItem == Window->Id && State.MousePressed &&
      // Ensure that there isn't another active widget.
      State.ActiveItem == DEFAULT_WIDGET_ID)
  {
    State.ActiveItem = Window->Id;

    // When we start dragging "pin" the point where the drag started.
    // This allows us to calculate the delta between the mouse position and
    // the top left corner of the frame's rect without them going out of sync.
    State.DragOffsetX = State.MouseX - Window->X;
    State.DragOffsetY = State.MouseY - Window->Y;
  }

  if (State.ActiveItem == Window->Id && State.HotItem == Window->Id)
  {
    // Update the position of the frame by dragging it around until the mouse is released.
    if (State.MouseReleased)
    {
      State.ActiveItem = DEFAULT_WIDGET_ID;
    }
    else
    {
      Window->X = State.MouseX - State.DragOffsetX;
      Window->Y = State.MouseY - State.DragOffsetY;
    }
  }

  const jui_rect Rect = {.X = Window->X, .Y = Window->Y, .W = W, .H = H};
  jui_sdl_draw_frame(Window->Id, Text, &Rect);
}

bool jui_begin(const char* Text, const f32 W, const f32 H, const i8 WindowIndex)
{
  if (WindowIndex == 0)
  {
    jui_make_window(&State.Windows[0], Text, W, H);
  }
  else if (WindowIndex == 1)
  {
    jui_make_window(&State.Windows[1], Text, W, H);
  }
  else if (WindowIndex == 2)
  {
    jui_make_window(&State.Windows[2], Text, W, H);
  }
  return true;
}

void jui_end() {}

void jui_label(const char* Text, const f32 X, const f32 Y)
{
  assert(State.CurrentWindow != NULL);
  const f32 WindowPositionX = State.CurrentWindow->X;
  const f32 WindowPositionY = State.CurrentWindow->Y;

  const f32 LabelPositionX = WindowPositionX + X;
  const f32 LabelPositionY = WindowPositionY + Y;

  sdl_draw_text_impl(SDLState.Renderer, Text, LabelPositionX, LabelPositionY);
}

bool jui_button(const char* Text, const f32 X, const f32 Y, const f32 W, const f32 H)
{
  bool Result = false;

  // TODO(jake): Add a scope for this ID (e.g., a window/frame or a scope manually defined by the user).
  const jui_id Id = hash_djb2((uchar*)Text);

  // TEMP(jake): This should become a lookup into a vector or map once we have dynamic windowing
  assert(State.CurrentWindow != NULL);
  const f32 WindowPositionX = State.CurrentWindow->X;
  const f32 WindowPositionY = State.CurrentWindow->Y;

  // TODO(jake): Need to figure out layouting. Right now it just depends on a single frame.
  //             Once we have a layout/position pass in we can also ensure this doesn't exceed the container.
  assert(WindowPositionX != UNINITIALIZED_FRAME_POSITION && WindowPositionY != UNINITIALIZED_FRAME_POSITION);
  const f32 ButtonPositionX = WindowPositionX + X;
  const f32 ButtonPositionY = WindowPositionY + Y;

  perform_hit_test(Id, ButtonPositionX, ButtonPositionY, W, H);

  // The button is in it's "active" state when it's hot and the mouse is down.
  if (State.HotItem == Id && State.MouseDown &&
      // Ensure that there isn't another active widget.
      (State.ActiveItem == DEFAULT_WIDGET_ID
       // HACK(jake): ...unless the active widget is the current window. This is a hack because
       //             the framework doesn't have proper z-indexing yet. Theoretically any two
       //             widgets that hit test may trigger this prior to a proper z-index being considered, I think.
       || State.ActiveItem == State.CurrentWindow->Id))
  {
    State.ActiveItem = Id;
  }

  // The button is pressed when it's active, hot, and the mouse is released this frame.
  // This is a "completed" interaction, so reset the active item as well.
  if (State.ActiveItem == Id && State.HotItem == Id && State.MouseReleased)
  {
    Result = true;
    State.ActiveItem = DEFAULT_WIDGET_ID;
  }
  // The hot item does not match the active item, and the active item this widget. This is
  // equivalent to the mouse leaving an active item mid-interaction (e.g., the mouse leaving
  // the button mid-press) and is often treated as a "cancellation" of the user's interaction.
  //
  // Most widgets will require some check like this. The clickable area for a draggable component
  // is one exemption because the mouse 'locks' while you're dragging it, not requiring a cancel.
  else if (State.HotItem != Id && State.ActiveItem == Id && State.MouseReleased)
  {
    State.ActiveItem = DEFAULT_WIDGET_ID;
  }

  const jui_rect Rect = {.X = ButtonPositionX, .Y = ButtonPositionY, .W = W, .H = H};
  jui_sdl_draw_button(Id, Text, &Rect);

  return Result;
}
