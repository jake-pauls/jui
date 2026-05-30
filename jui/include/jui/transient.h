#pragma once

typedef union SDL_Event SDL_Event;

/// Platform (SDL)

void jui_sdl_consume_event(SDL_Event* event);

/// Renderer (SDL)
