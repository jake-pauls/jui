#include <jui/jui.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>

int main()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow("JUI Sandbox", 800, 600, SDL_WINDOW_RESIZABLE);

  if (!window)
  {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

  if (!renderer)
  {
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  bool isRunning = true;
  SDL_Event event;
  while (isRunning)
  {
    while (SDL_PollEvent(&event))
    {
      jui_sdl_consume_event(&event);

      if (event.type == SDL_EVENT_QUIT)
      {
        isRunning = false;
      }
    }

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderClear(renderer);

    // Render
    {
      SDL_FRect rect;
      rect.x = 0;
      rect.y = 0;
      rect.w = 64;
      rect.h = 48;
      SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
      SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
