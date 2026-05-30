#include "jui/transient.h"

#include "SDL3/SDL_render.h"

#include <SDL3/SDL_events.h>

typedef struct jui_state
{
  int MouseX;
  int MouseY;
  int MouseDown;

  int HotItem;
  int ActiveItem;
} jui_state;

static jui_state state = {
    .MouseX = 0,
    .MouseY = 0,
    .MouseDown = 0,
    .HotItem = 0,
    .ActiveItem = 0,
};

// TODO(jake): This eventually should be split from the GUI implementation
void jui_sdl_consume_event(SDL_Event* event)
{
  switch (event->type)
  {
  case SDL_EVENT_MOUSE_MOTION:
    state.MouseX = event->motion.x;
    state.MouseY = event->motion.y;
    break;
  case SDL_EVENT_MOUSE_BUTTON_DOWN:
    if (event->button.button == SDL_BUTTON_LEFT)
    {
      state.MouseDown = 1;
    }
    break;
  case SDL_EVENT_MOUSE_BUTTON_UP:
    if (event->button.button == SDL_BUTTON_LEFT)
    {
      state.MouseDown = 0;
    }
    break;
  }
}

void jui_sdl_draw_rect(SDL_Renderer* renderer, f32 x, f32 y, f32 w, f32 h, u8 r, u8 g, u8 b, u8 a)
{
  SDL_FRect rect;
  // TODO(jake): Temp code to test ui_state
  // rect.x = state.MouseX - w / 2;
  // rect.y = state.MouseY - h / 2;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  SDL_SetRenderDrawColor(renderer, r, g, b, 255);
  SDL_RenderFillRect(renderer, &rect);
}
