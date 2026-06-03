#include "jui/transient.h"

#include "core/hash.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>

#define DEFAULT_WIDGET_ID (0)

typedef struct jui_state jui_state;
struct jui_state
{
  f32 MouseX;
  f32 MouseY;
  bool MouseDown;
  bool MouseUp;

  u64 HotItem;
  u64 ActiveItem;
};

typedef struct jui_sdl_state jui_sdl_state;
struct jui_sdl_state
{
  SDL_Renderer* Renderer;
};

static jui_state State =
{
    .MouseX = 0,
    .MouseY = 0,
    .MouseDown = false,
    .MouseUp = false,
    .HotItem = DEFAULT_WIDGET_ID,
    .ActiveItem = DEFAULT_WIDGET_ID,
};

static jui_sdl_state SDLState =
{
  .Renderer = NULL,
};

/* Platform (SDL) */

void jui_sdl_consume_event(const SDL_Event* Event)
{
  // Reset the mouse up state every time an event is consumed
  State.MouseUp = false;

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
    }
    break;
  case SDL_EVENT_MOUSE_BUTTON_UP:
    if (Event->button.button == SDL_BUTTON_LEFT)
    {
      if (State.MouseDown)
      {
        State.MouseUp = true;
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

void jui_sdl_draw_rect(SDL_Renderer* Renderer,
  const f32 X, const f32 Y,
  const f32 W, const f32 H,
  const u8 R, const u8 G, const u8 B, const u8 A)
{
  SDL_FRect Rect;
  Rect.x = X;
  Rect.y = Y;
  Rect.w = W;
  Rect.h = H;
  SDL_SetRenderDrawColor(Renderer, R, G, B, A);
  SDL_RenderFillRect(Renderer, &Rect);
}

void jui_sdl_draw_frame(SDL_Renderer* Renderer, const u64 Id,
  const f32 X, const f32 Y,
  const f32 W, const f32 H)
{
  if (State.ActiveItem == Id)
  {
    // Active color (95% pink)
    jui_sdl_draw_rect(Renderer, X, Y, W, H, 255, 230, 242, 255);
  }
  else if (State.HotItem == Id)
  {
    // Hot/hover color (90% pink)
    jui_sdl_draw_rect(Renderer, X, Y, W, H, 255, 204, 229, 255);
  }
  else
  {
    // Inactive (80% pink)
    jui_sdl_draw_rect(Renderer, X, Y, W, H, 255, 153, 202, 255);
  }
}

void jui_sdl_draw_button(SDL_Renderer* Renderer, const u64 Id,
  const f32 X, const f32 Y,
  const f32 W, const f32 H)
{
  if (State.ActiveItem == Id)
  {
    // Active color (green)
    jui_sdl_draw_rect(Renderer, X, Y, W, H, 0, 255, 0, 255);
  }
  else if (State.HotItem == Id)
  {
    // Hot/hover color (cyan)
    jui_sdl_draw_rect(Renderer, X, Y, W, H, 0, 255, 255, 255);
  }
  else
  {
    // Inactive (blue)
    jui_sdl_draw_rect(Renderer, X, Y, W, H, 0, 0, 255, 255);
  }
}

/* API */

/// \brief Simple hit test function for a box defined with an \p X and \p Y coordinate as well
///        as a provided width and height that is performed against the global state.
/// \param X The x coordinate used for the hit test.
/// \param Y The y coordinate used for the hit test.
/// \param W The width used for the hit test.
/// \param H The height used for the hit test.
/// \returns <c>true</c> if the hit test passed for the global state, otherwise <c>false</c>.
bool jui_hit_test(const f32 X, const f32 Y, const f32 W, const f32 H)
{
  if (State.MouseX < X || State.MouseY < Y || State.MouseX >= X + W || State.MouseY >= Y + H)
  {
    return false;
  }
  return true;
}

bool jui_begin(const char* Name, const f32 X, const f32 Y, const f32 W, const f32 H)
{
  // TODO(jake): Push this ID onto the frame stack.
  const u64 Id = core_hash_djb2((uchar*)Name);

  // Only set an item as hot if we pass the hit test and do not have a currently active item.
  // Useful for preventing a click event on another widget if the user clicks and drags off the button.
  if (jui_hit_test(X, Y, W, H))
  {
    if (State.ActiveItem == DEFAULT_WIDGET_ID)
    {
      State.HotItem = Id;
    }
  }
  // If we fail the hit test and the current item is hot, reset it.
  else if (State.HotItem == Id)
  {
    State.HotItem = DEFAULT_WIDGET_ID;
  }

  // TODO(jake): Make a clearer division between the renderer and the GUI
  jui_sdl_draw_frame(SDLState.Renderer, Id, X, Y, W, H);

  return true;
}

void jui_end()
{
  // TODO(jake): Pop the frame stack.
}

bool jui_button(const char* Text, const f32 X, const f32 Y, const f32 W, const f32 H)
{
  bool Result = false;

  // TODO(jake): Add a scope for this ID (e.g., a window/frame or a scope manually defined by the user).
  const u64 Id = core_hash_djb2((uchar*)Text);

  if (State.ActiveItem == Id)
  {
    if (State.MouseUp)
    {
      if (State.HotItem == Id)
      {
        Result = true;
      }
      State.ActiveItem = DEFAULT_WIDGET_ID;
    }
  }
  else if (State.HotItem == Id)
  {
    if (State.MouseDown)
    {
      State.ActiveItem = Id;
    }
  }

  // Only set an item as hot if we pass the hit test and do not have a currently active item.
  // Useful for preventing a click event on another widget if the user clicks and drags off the button.
  if (jui_hit_test(X, Y, W, H))
  {
    if (State.ActiveItem == DEFAULT_WIDGET_ID)
    {
      State.HotItem = Id;
    }
  }
  // If we fail the hit test and the current item is hot, reset it.
  else if (State.HotItem == Id)
  {
    State.HotItem = DEFAULT_WIDGET_ID;
  }

  // TODO(jake): Make a clearer division between the renderer and the GUI
  jui_sdl_draw_button(SDLState.Renderer, Id, X, Y, W, H);

  return Result;
}

