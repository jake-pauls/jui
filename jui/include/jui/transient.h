#pragma once

// TODO(jake): Is it possible to avoid this?
#include <tak/types.h>

typedef union SDL_Event SDL_Event;
typedef struct SDL_Renderer SDL_Renderer;

/// Platform (SDL)

void jui_sdl_consume_event(SDL_Event* event);

/// Renderer (SDL)

void jui_sdl_draw_rect(SDL_Renderer* renderer, f32 x, f32 y, f32 w, f32 h, u8 r, u8 g, u8 b, u8 a);