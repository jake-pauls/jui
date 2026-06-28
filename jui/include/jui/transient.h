#pragma once

#include "jui/types.h"

#include <stdbool.h>

typedef union SDL_Event SDL_Event;
typedef struct SDL_Renderer SDL_Renderer;

/* Lifecycle (General) */

void jui_start_frame();
void jui_render();

void jui_teardown();

/* Platform (SDL) */

void jui_sdl_consume_event(const SDL_Event* Event);

/* Renderer (SDL) */

void jui_sdl_init(SDL_Renderer* Renderer);

/* Widgets */

void jui_label(const char* Text, f32 X, f32 Y);
void jui_panel(const char* Text, f32 W, f32 H);
bool jui_button(const char* Text, f32 X, f32 Y, f32 W, f32 H);
