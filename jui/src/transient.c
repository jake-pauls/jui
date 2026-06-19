#include "jui/transient.h"

#include "jui/core.h"
#include "jui/core_memory.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <assert.h>
#include <stdlib.h>

#define MAXIMUM_WINDOW_COUNT (3)
#define MAXIMUM_DRAW_LIST_SIZE_PER_FRAME (50)
// TODO(jake): Implement byte helpers - (~8kB each)
#define MAXIMUM_FRAME_ARENA_SIZE_BYTES (8192)
#define MAXIMUM_APP_ARENA_SIZE_BYTES (8192)

#define DEFAULT_WIDGET_ID (0)
#define UNINITIALIZED_FRAME_POSITION (-1)

/* Forward Declarations */

typedef enum jui_draw_command_parameters_type jui_draw_command_parameters_type;
typedef struct jui_draw_command_parameters jui_draw_command_parameters;
typedef void (*jui_draw_command)(const jui_draw_command_parameters*);
typedef struct jui_draw_list jui_draw_list;
typedef struct sdl_draw_rect_params sdl_draw_rect_params;
typedef struct sdl_draw_text_params sdl_draw_text_params;

typedef struct jui_window jui_window;
typedef struct jui_window_list jui_window_list;

typedef struct jui_state jui_state;
typedef struct jui_sdl_state jui_sdl_state;

void sdl_draw_impl(const jui_draw_command_parameters* Parameters);
bool hit_test(const f32 X, const f32 Y, const f32 W, const f32 H);

/* UI Structures */

/// \brief Represents a window widget in jui. Contains a draw list of command
///        for rendering each widget in the window that are executed at the end
///        of the frame.
struct jui_window
{
  jui_id Id;
  jui_draw_list* DrawList;
  jui_rect Rect;
  f32 Z;
};

struct jui_window_list
{
  jui_window* Windows[MAXIMUM_WINDOW_COUNT];
  u32 Count;
};

/* Rendering */

enum jui_draw_command_parameters_type
{
  DrawCommandParameters_DrawRect,
  DrawCommandParameters_DrawText
};

struct jui_draw_command_parameters
{
  void* Data;
  jui_draw_command_parameters_type Type;
};

struct jui_draw_list
{
  jui_draw_command_parameters* Parameters[MAXIMUM_DRAW_LIST_SIZE_PER_FRAME];
  u32 Count;
};

struct sdl_draw_rect_params
{
  jui_rect Rect;
  jui_color Color;
};

struct sdl_draw_text_params
{
  const char* Text;
  jui_v2 Position;
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

  arena AppArena;
  arena FrameArena;

  // TODO(jake): Abstract this into a different data structure (e.g., a list or map with positions per frame).
  //             I think that having frames be nested (in effect, layouts) is a much more idiomatic approach.
  //             If we have bespoke frames finitely containing widgets, or, `begin`/`end` calls, we make
  //             way for frames to be something other than just another bespoke widget. If we just generally
  //             'layout' every widget and make the frame/window another widget with other widgets laid out
  //             inside it, the architecture might be much cleaner.
  jui_window* CurrentWindow;
  jui_window* HoveredWindow;
  jui_window_list WindowList;

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
    .AppArena =
        {
            .Base = NULL,
            .Position = 0,
        },
    .FrameArena =
        {
            .Base = NULL,
            .Position = 0,
        },
    .HoveredWindow = NULL,
    .CurrentWindow = NULL,
    .WindowList =
        {
            .Windows = {0},
            .Count = 0,
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
  arena_alloc(&State.AppArena, MAXIMUM_APP_ARENA_SIZE_BYTES);
  arena_alloc(&State.FrameArena, MAXIMUM_FRAME_ARENA_SIZE_BYTES);
}

void jui_start_frame()
{
  // At the start of each frame clear the frame-by-frame memory arena.
  arena_clear(&State.FrameArena);

  // The rest of the per-frame operations assume that at least one window has been created.
  if (State.WindowList.Count == 0)
  {
    return;
  }

  // Reset the window draw list for each window.
  for (size_t WindowIdx = 0; WindowIdx < State.WindowList.Count; ++WindowIdx)
  {
    const jui_window* Window = State.WindowList.Windows[WindowIdx];
    if (Window != NULL)
    {
      Window->DrawList->Count = 0;
    }
  }

  // Determine the highest Z-ordered window that is hovered by the mouse
  State.HoveredWindow = NULL;
  for (size_t WindowIdx = 0; WindowIdx < State.WindowList.Count; ++WindowIdx)
  {
    jui_window* Window = State.WindowList.Windows[WindowIdx];

    // Check if this window passes the hit test.
    if (hit_test(Window->Rect.X, Window->Rect.Y, Window->Rect.W, Window->Rect.H))
    {
      // Any window that passes the hit test can become the hovered window, regardless of Z-order.
      if (State.HoveredWindow == NULL)
      {
        State.HoveredWindow = Window;
      }
      // Replace the currently hovered window if the window we're iterating is "closer".
      else if (State.HoveredWindow->Z > Window->Z)
      {
        State.HoveredWindow = Window;
      }
    }
  }
}

void jui_teardown()
{
  arena_clear(&State.AppArena);
  arena_free(&State.AppArena);
  arena_clear(&State.FrameArena);
  arena_free(&State.FrameArena);
}

void jui_render()
{
  for (size_t WindowIdx = 0; WindowIdx < State.WindowList.Count; ++WindowIdx)
  {
    const jui_window* Window = State.WindowList.Windows[WindowIdx];
    if (Window == NULL)
    {
      continue;
    }

    // Execute each command in the draw list for each window.
    for (size_t Idx = 0; Idx < Window->DrawList->Count; ++Idx)
    {
      const jui_draw_list* DrawList = Window->DrawList;
      const jui_draw_command_parameters* Parameter = DrawList->Parameters[Idx];
      sdl_draw_impl(Parameter);
    }
  }

  // TODO(jake): Maybe we don't have to sort the windows by their Z-index before every draw.
  for (size_t I = 0; I < State.WindowList.Count - 1; ++I)
  {
    jui_window* One = State.WindowList.Windows[I];
    jui_window* Two = State.WindowList.Windows[I + 1];
    if (One->Z < Two->Z)
    {
      State.WindowList.Windows[I] = Two;
      State.WindowList.Windows[I + 1] = One;
    }
  }
}

/* Renderer (SDL) */

void jui_sdl_init(SDL_Renderer* Renderer)
{
  SDLState.Renderer = Renderer;

  // NOTE(jake): Adding this in the backend init call for now because I don't think the user should have to care about calling it.
  jui_init();
}

void sdl_draw_rect_impl(SDL_Renderer* Renderer, const jui_rect Rect, const jui_color Color)
{
  SDL_FRect FRect;
  FRect.x = Rect.X;
  FRect.y = Rect.Y;
  FRect.w = Rect.W;
  FRect.h = Rect.H;
  SDL_SetRenderDrawColor(Renderer, Color.R, Color.G, Color.B, Color.A);
  SDL_RenderFillRect(Renderer, &FRect);
}

void sdl_draw_text_impl(SDL_Renderer* Renderer, const char* Text, const jui_v2 Position)
{
  SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
  SDL_RenderDebugText(Renderer, Position.X, Position.Y, Text);
}

void sdl_draw_impl(const jui_draw_command_parameters* Parameters)
{
  switch (Parameters->Type)
  {
  case DrawCommandParameters_DrawRect:
  {
    const sdl_draw_rect_params* DrawRectParams = Parameters->Data;
    sdl_draw_rect_impl(SDLState.Renderer, DrawRectParams->Rect, DrawRectParams->Color);
    break;
  }
  case DrawCommandParameters_DrawText:
  {
    const sdl_draw_text_params* DrawTextParams = Parameters->Data;
    sdl_draw_text_impl(SDLState.Renderer, DrawTextParams->Text, DrawTextParams->Position);
    break;
  }
  }
}

void jui_create_draw_rect_command(const jui_rect Rect, const jui_color Color)
{
  sdl_draw_rect_params* DrawRectParams = PUSH_STRUCT_ZERO(&State.FrameArena, sdl_draw_rect_params);

  DrawRectParams->Rect.X = Rect.X;
  DrawRectParams->Rect.Y = Rect.Y;
  DrawRectParams->Rect.W = Rect.W;
  DrawRectParams->Rect.H = Rect.H;

  DrawRectParams->Color.R = Color.R;
  DrawRectParams->Color.G = Color.G;
  DrawRectParams->Color.B = Color.B;
  DrawRectParams->Color.A = Color.A;

  jui_draw_command_parameters* CommandParams = PUSH_STRUCT_ZERO(&State.FrameArena, jui_draw_command_parameters);
  CommandParams->Data = DrawRectParams;
  CommandParams->Type = DrawCommandParameters_DrawRect;

  assert(State.CurrentWindow->DrawList->Count < MAXIMUM_DRAW_LIST_SIZE_PER_FRAME);
  State.CurrentWindow->DrawList->Parameters[State.CurrentWindow->DrawList->Count] = CommandParams;
  State.CurrentWindow->DrawList->Count++;
}

void jui_create_draw_text_command(const jui_v2 Position, const char* const Text)
{
  sdl_draw_text_params* DrawTextParams = PUSH_STRUCT_ZERO(&State.FrameArena, sdl_draw_text_params);

  // TODO(jake): Need to make some string abstractions.
  char* AllocatedText = PUSH_ARRAY_ZERO(&State.FrameArena, char, strlen(Text) + 1);
  strcpy(&AllocatedText[0], &Text[0]);

  DrawTextParams->Text = AllocatedText;
  DrawTextParams->Position.X = Position.X + 2;
  DrawTextParams->Position.Y = Position.Y + 2;

  jui_draw_command_parameters* CommandParams = PUSH_STRUCT_ZERO(&State.FrameArena, jui_draw_command_parameters);
  CommandParams->Data = DrawTextParams;
  CommandParams->Type = DrawCommandParameters_DrawText;

  assert(State.CurrentWindow->DrawList->Count < MAXIMUM_DRAW_LIST_SIZE_PER_FRAME);
  State.CurrentWindow->DrawList->Parameters[State.CurrentWindow->DrawList->Count] = CommandParams;
  State.CurrentWindow->DrawList->Count++;
}

void jui_sdl_draw_frame(const jui_id Id, const char* Text, const jui_rect Rect)
{
  if (State.ActiveItem == Id)
  {
    // Active color (95% pink)
    const jui_color Color = {.R = 255, .G = 230, .B = 242, .A = 255};
    jui_create_draw_rect_command(Rect, Color);
  }
  else if (State.HotItem == Id)
  {
    // Hot/hover color (90% pink)
    const jui_color Color = {.R = 255, .G = 204, .B = 229, .A = 255};
    jui_create_draw_rect_command(Rect, Color);
  }
  else
  {
    // Inactive (80% pink)
    const jui_color Color = {.R = 255, .G = 153, .B = 202, .A = 255};
    jui_create_draw_rect_command(Rect, Color);
  }

  // TODO(jake): Proper layouts, right now just add some arbitrary margin around the frame text.
  const jui_v2 TextPosition = {.V = {Rect.X + 2, Rect.Y + 2}};
  jui_create_draw_text_command(TextPosition, Text);
}

void jui_sdl_draw_button(const jui_id Id, const char* Text, const jui_rect Rect)
{
  if (State.ActiveItem == Id)
  {
    // Active color (green)
    const jui_color Color = {.R = 0, .G = 255, .B = 0, .A = 255};
    jui_create_draw_rect_command(Rect, Color);
  }
  else if (State.HotItem == Id)
  {
    // Hot/hover color (cyan)
    const jui_color Color = {.R = 0, .G = 255, .B = 255, .A = 255};
    jui_create_draw_rect_command(Rect, Color);
  }
  else
  {
    // Inactive (blue)
    const jui_color Color = {.R = 0, .G = 0, .B = 255, .A = 255};
    jui_create_draw_rect_command(Rect, Color);
  }

  // TODO(jake): Proper layouts, right now it's just an arbitrary position near the center of the button.
  //             Note that the X coordinate is really all that's needed now because text is anchored on the
  //             top right point of the texture.
  const jui_v2 TextPosition = {.V = {Rect.X, Rect.H / 2.0f + Rect.Y}};
  jui_create_draw_text_command(TextPosition, Text);
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

/// \brief Performs a hit test on a jui widget and updates the internal state depending on the result.
/// \param Id The unique identifier of the widget that the hit is being tested against.
/// \param X The x coordinate used for the hit test.
/// \param Y The y coordinate used for the hit test.
/// \param W The width used for the hit test.
/// \param H The height used for the hit test.
bool perform_widget_hit_test(const jui_id Id, const f32 X, const f32 Y, const f32 W, const f32 H)
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

/// \brief Looks up a window by ID in the current window list.
/// \param Id The identifier of the window to lookup.
/// \return Pointer to the window that matches the provided identifier, <c>NULL</c> if no window was found matching the provided identifier.
jui_window* get_window_by_id(const jui_id Id)
{
  if (State.WindowList.Count == 0)
  {
    return NULL;
  }

  for (size_t WindowIdx = 0; WindowIdx < State.WindowList.Count; ++WindowIdx)
  {
    jui_window* ExistingWindow = State.WindowList.Windows[WindowIdx];
    if (ExistingWindow != NULL && ExistingWindow->Id == Id)
    {
      return ExistingWindow;
    }
  }

  return NULL;
}

/// \brief Performs a check if the window that is logically hovered is equal to the current window being processed.
/// \remarks If check passes it's worthwhile for the widget to perform interaction logic (e.g., hit tests or checks).
///          Any widget that is not in a hovered window is not worthwhile for performing such checks because it isn't
///          possible that the mouse can interact with it this frame.
bool is_in_hovered_window()
{
  return State.HoveredWindow != NULL && State.CurrentWindow != NULL && State.HoveredWindow->Id == State.CurrentWindow->Id;
}

/* Widgets */

bool jui_begin(const char* Text, const f32 W, const f32 H)
{
  const jui_id Id = hash_djb2((uchar*)Text);

  jui_window* Window = get_window_by_id(Id);
  if (!Window)
  {
    Window = PUSH_STRUCT_ZERO(&State.AppArena, jui_window);

    Window->Id = Id;
    Window->Rect.X = 0;
    Window->Rect.Y = 0;
    Window->Rect.W = W;
    Window->Rect.H = H;
    // BUG(jake): When windows are initialized they all have equivalent Z values, so the first one drawn is
    //            always the hovered one despite potentially not being at the top.
    Window->Z = 0.0f;

    jui_draw_list* NewDrawList = PUSH_STRUCT_ZERO(&State.AppArena, jui_draw_list);
    Window->DrawList = NewDrawList;

    State.WindowList.Windows[State.WindowList.Count] = Window;
    State.WindowList.Count++;
  }

  State.CurrentWindow = Window;

  // Prevent triggering interaction logic if the current window is the one being hovered.
  if (is_in_hovered_window())
  {
    perform_widget_hit_test(Window->Id, Window->Rect.X, Window->Rect.Y, W, H);

    if (State.HotItem == Window->Id && State.MousePressed &&
        // Ensure that there isn't another active widget.
        State.ActiveItem == DEFAULT_WIDGET_ID)
    {
      State.ActiveItem = Window->Id;

      // Once a window is pressed, update it's Z-index and the Z-index of all other windows.
      // Maybe there's a more ergonomic solution that doesn't subsequently update all other windows.
      {
        // Set this window to the last in the draw order (the top).
        Window->Z = 0.0f;

        // Infinitely increment the Z-index of other windows as a natural "history" for which one was on top.
        for (size_t I = 0; I < State.WindowList.Count; ++I)
        {
          jui_window* Next = State.WindowList.Windows[I];
          if (Next != NULL && Next->Id != Id)
          {
            Next->Z++;
          }
        }
      }

      // When we start dragging "pin" the point where the drag started.
      // This allows us to calculate the delta between the mouse position and
      // the top left corner of the frame's rect without them going out of sync.
      State.DragOffsetX = State.MouseX - Window->Rect.X;
      State.DragOffsetY = State.MouseY - Window->Rect.Y;
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
        Window->Rect.X = State.MouseX - State.DragOffsetX;
        Window->Rect.Y = State.MouseY - State.DragOffsetY;
      }
    }
  }
  // Reset the hot item because it's not being hovered.
  else if (State.HotItem == Id)
  {
    State.HotItem = DEFAULT_WIDGET_ID;
  }

  jui_sdl_draw_frame(Window->Id, Text, Window->Rect);

  return true;
}

void jui_end() {}

void jui_label(const char* Text, const f32 X, const f32 Y)
{
  assert(State.CurrentWindow != NULL);
  const f32 WindowPositionX = State.CurrentWindow->Rect.X;
  const f32 WindowPositionY = State.CurrentWindow->Rect.Y;

  const jui_v2 LabelPosition = {.V = {WindowPositionX + X, WindowPositionY + Y}};
  sdl_draw_text_impl(SDLState.Renderer, Text, LabelPosition);
}

bool jui_button(const char* Text, const f32 X, const f32 Y, const f32 W, const f32 H)
{
  bool Result = false;

  // TODO(jake): Add a scope for this ID (e.g., a window/frame or a scope manually defined by the user).
  const jui_id Id = hash_djb2((uchar*)Text);

  assert(State.CurrentWindow != NULL);
  const f32 ButtonPositionX = State.CurrentWindow->Rect.X + X;
  const f32 ButtonPositionY = State.CurrentWindow->Rect.Y + Y;
  const jui_rect Rect = {.X = ButtonPositionX, .Y = ButtonPositionY, .W = W, .H = H};

  // Prevent triggering interaction logic if the current window is the one being hovered.
  if (is_in_hovered_window())
  {
    perform_widget_hit_test(Id, ButtonPositionX, ButtonPositionY, W, H);

    // The button is in it's "active" state when it's hot and the mouse is down.
    if (State.HotItem == Id && State.MouseDown &&
        // Ensure that there isn't another active widget.
        (State.ActiveItem == DEFAULT_WIDGET_ID
         // HACK(jake): ...unless the active widget is the current window. This is a hack to allow button presses
         //             while the window they're laid out in is also "active". Since the window executes first in
         //             a frame it'll register itself as "active".
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
  }
  // Reset the hot item because it's not being hovered.
  else if (State.HotItem == Id)
  {
    State.HotItem = DEFAULT_WIDGET_ID;
  }

  jui_sdl_draw_button(Id, Text, Rect);

  return Result;
}
