#include "jui/transient.h"

#include "jui/core.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <assert.h>

#define DEFAULT_WIDGET_ID (0)
#define UNINITIALIZED_FRAME_POSITION (-1)

typedef struct jui_state jui_state;
struct jui_state
{
  f32 MouseX;
  f32 MouseY;
  f32 DragOffsetX;
  f32 DragOffsetY;

  bool MouseDown;
  bool MousePressed;
  bool MouseReleased;

  // TODO(jake): Abstract this into a different data structure (e.g., a list or map with positions per frame).
  //             I think that having frames be nested (in effect, layouts) is a much more idiomatic approach.
  //             If we have bespoke frames finitely containing widgets, or, `begin`/`end` calls, we make
  //             way for frames to be something other than just another bespoke widget. If we just generally
  //             'layout' every widget and make the frame/window another widget with other widgets laid out
  //             inside it, the architecture might be much cleaner.
  struct
  {
    f32 FramePositionX;
    f32 FramePositionY;
  };

  jui_id HotItem;
  jui_id ActiveItem;
};

typedef struct jui_sdl_state jui_sdl_state;
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
    .FramePositionX = UNINITIALIZED_FRAME_POSITION,
    .FramePositionY = UNINITIALIZED_FRAME_POSITION,
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

/* Renderer (SDL) */

void jui_sdl_init(SDL_Renderer* Renderer)
{
  SDLState.Renderer = Renderer;
}

void jui_sdl_draw_rect(SDL_Renderer* Renderer, const jui_rect* const Rect, const u8 R, const u8 G, const u8 B, const u8 A)
{
  SDL_FRect FRect;
  FRect.x = Rect->X;
  FRect.y = Rect->Y;
  FRect.w = Rect->W;
  FRect.h = Rect->H;
  SDL_SetRenderDrawColor(Renderer, R, G, B, A);
  SDL_RenderFillRect(Renderer, &FRect);
}

void jui_sdl_draw_text(SDL_Renderer* Renderer, const char* Text, const f32 X, const f32 Y)
{
  SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
  SDL_RenderDebugText(Renderer, X, Y, Text);
}

void jui_sdl_draw_frame(SDL_Renderer* Renderer, const jui_id Id, const char* Text, const jui_rect* const Rect)
{
  if (State.ActiveItem == Id)
  {
    // Active color (95% pink)
    jui_sdl_draw_rect(Renderer, Rect, 255, 230, 242, 255);
  }
  else if (State.HotItem == Id)
  {
    // Hot/hover color (90% pink)
    jui_sdl_draw_rect(Renderer, Rect, 255, 204, 229, 255);
  }
  else
  {
    // Inactive (80% pink)
    jui_sdl_draw_rect(Renderer, Rect, 255, 153, 202, 255);
  }

  // TODO(jake): Proper layouting, right now just add some arbitrary margin around the frame text.
  jui_sdl_draw_text(Renderer, Text, Rect->X + 2, Rect->Y + 2);
}

void jui_sdl_draw_button(SDL_Renderer* Renderer, const jui_id Id, const char* Text, const jui_rect* const Rect)
{
  if (State.ActiveItem == Id)
  {
    // Active color (green)
    jui_sdl_draw_rect(Renderer, Rect, 0, 255, 0, 255);
  }
  else if (State.HotItem == Id)
  {
    // Hot/hover color (cyan)
    jui_sdl_draw_rect(Renderer, Rect, 0, 255, 255, 255);
  }
  else
  {
    // Inactive (blue)
    jui_sdl_draw_rect(Renderer, Rect, 0, 0, 255, 255);
  }

  // TODO(jake): Proper layouting, right now it's just an arbitrary position near the center of the button.
  //             Note that the X coordinate is really all that's needed now because text is anchored on the
  //             top right point of the texture.
  const f32 TextX = Rect->X;
  const f32 TextY = Rect->H / 2.0f + Rect->Y;
  jui_sdl_draw_text(Renderer, Text, TextX, TextY);
}

/* API */

/// \brief Simple hit test function for a box defined with an \p X and \p Y coordinate as well
///        as a provided width and height that is performed against the global state.
/// \param X The x coordinate used for the hit test.
/// \param Y The y coordinate used for the hit test.
/// \param W The width used for the hit test.
/// \param H The height used for the hit test.
/// \returns `true` if the hit test passed for the global state, otherwise `false`.
bool jui_hit_test(const f32 X, const f32 Y, const f32 W, const f32 H)
{
  return !(State.MouseX < X || State.MouseY < Y || State.MouseX >= X + W || State.MouseY >= Y + H);
}

/// \brief Performs a hit test and updates the internal state depending on the result.
/// \param Id The unique identifier of the widget that the hit is being tested against.
/// \param X The x coordinate used for the hit test.
/// \param Y The y coordinate used for the hit test.
/// \param W The width used for the hit test.
/// \param H The height used for the hit test.
bool jui_perform_hit_test(const jui_id Id, const f32 X, const f32 Y, const f32 W, const f32 H)
{
  // Only set an item as hot if we pass the hit test and do not have a currently active item.
  // Useful for preventing a click event on another widget if the user clicks and drags off the button.
  if (jui_hit_test(X, Y, W, H))
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

bool jui_begin(const char* Text, const f32 W, const f32 H)
{
  // TODO(jake): Remove this when we figure out layouting.
  State.FramePositionX = State.FramePositionX == UNINITIALIZED_FRAME_POSITION ? 0 : State.FramePositionX;
  State.FramePositionY = State.FramePositionY == UNINITIALIZED_FRAME_POSITION ? 0 : State.FramePositionY;

  // TODO(jake): Push this ID onto the frame stack.
  const jui_id Id = hash_djb2((uchar*)Text);

  jui_perform_hit_test(Id, State.FramePositionX, State.FramePositionY, W, H);

  if (State.HotItem == Id && State.MousePressed)
  {
    State.ActiveItem = Id;

    // When we start dragging "pin" the point where the drag started.
    // This allows us to calculate the delta between the mouse position and
    // the top left corner of the frame's rect without them going out of sync.
    State.DragOffsetX = State.MouseX - State.FramePositionX;
    State.DragOffsetY = State.MouseY - State.FramePositionY;
  }

  if (State.ActiveItem == Id && State.HotItem == Id)
  {
    // Update the position of the frame by dragging it around until the mouse is released.
    if (State.MouseReleased)
    {
      State.ActiveItem = DEFAULT_WIDGET_ID;
    }
    else
    {
      State.FramePositionX = State.MouseX - State.DragOffsetX;
      State.FramePositionY = State.MouseY - State.DragOffsetY;
    }
  }

  const jui_rect Rect = {
      .X = State.FramePositionX,
      .Y = State.FramePositionY,
      .W = W,
      .H = H,
  };
  jui_sdl_draw_frame(SDLState.Renderer, Id, Text, &Rect);

  return true;
}

void jui_end()
{
  // TODO(jake): Pop the frame stack.
}

void jui_label(const char* Text, const f32 X, const f32 Y)
{
  // TODO(jake): Add a scope for this ID (e.g., a window/frame or a scope manually defined by the user).
  const f32 LabelPositionX = State.FramePositionX + X;
  const f32 LabelPositionY = State.FramePositionY + Y;
  jui_sdl_draw_text(SDLState.Renderer, Text, LabelPositionX, LabelPositionY);
}

#include <stdio.h>
bool jui_button(const char* Text, const f32 X, const f32 Y, const f32 W, const f32 H)
{
  bool Result = false;

  // TODO(jake): Add a scope for this ID (e.g., a window/frame or a scope manually defined by the user).
  const jui_id Id = hash_djb2((uchar*)Text);

  // TODO(jake): Need to figure out layouting. Right now it just depends on a single frame.
  //             Once we have a layout/position pass in we can also ensure this doesn't exceed the container.
  assert(State.FramePositionX != UNINITIALIZED_FRAME_POSITION && State.FramePositionY != UNINITIALIZED_FRAME_POSITION);
  const f32 ButtonPositionX = State.FramePositionX + X;
  const f32 ButtonPositionY = State.FramePositionY + Y;

  jui_perform_hit_test(Id, ButtonPositionX, ButtonPositionY, W, H);

  // The button is in it's "active" state when it's hot and the mouse is down.
  if (State.HotItem == Id && State.MouseDown)
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

  const jui_rect Rect = {
      .X = ButtonPositionX,
      .Y = ButtonPositionY,
      .W = W,
      .H = H,
  };
  jui_sdl_draw_button(SDLState.Renderer, Id, Text, &Rect);

  return Result;
}
