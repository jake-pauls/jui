#include <jui/jui.h>
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>

int main()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window* Window = SDL_CreateWindow("jui sandbox", 800, 600, SDL_WINDOW_RESIZABLE);

  if (!Window)
  {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Renderer* Renderer = SDL_CreateRenderer(Window, NULL);

  if (!Renderer)
  {
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return 1;
  }

  // Init jui
  jui_sdl_init(Renderer);

  bool IsRunning = true;
  SDL_Event Event;
  while (IsRunning)
  {
    while (SDL_PollEvent(&Event))
    {
      jui_sdl_consume_event(&Event);

      if (Event.type == SDL_EVENT_QUIT || (Event.type == SDL_EVENT_KEY_DOWN && Event.key.key == SDLK_ESCAPE))
      {
        IsRunning = false;
      }
    }

    // Clear screen
    SDL_SetRenderDrawColor(Renderer, 25, 25, 25, 255);
    SDL_RenderClear(Renderer);

    // Render jui
    {
      if (jui_button(1, 0, 0, 64, 48))
      {
        printf("First button hit!\n");
      }

      if (jui_button(2, 0, 64, 64, 48))
      {
        printf("Second button hit!\n");
      }
    }

    SDL_RenderPresent(Renderer);
  }

  SDL_DestroyRenderer(Renderer);
  SDL_DestroyWindow(Window);
  SDL_Quit();

  return 0;
}