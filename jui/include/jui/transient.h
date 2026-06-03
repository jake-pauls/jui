#pragma once

// TODO(jake): Is it possible to avoid this?
#include "core/types.h"

#include <stdbool.h>

typedef union SDL_Event SDL_Event;
typedef struct SDL_Renderer SDL_Renderer;

/* Platform (SDL) */

void jui_sdl_consume_event(const SDL_Event* Event);

/* Renderer (SDL) */

void jui_sdl_init(SDL_Renderer* Renderer);

/* API */

bool jui_begin(const char* Name, f32 X, f32 Y, f32 W, f32 H);
void jui_end();
bool jui_button(const char* Text, f32 X, f32 Y, f32 W, f32 H);