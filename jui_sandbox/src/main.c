#include <SDL3/SDL.h>
#include <jui/jui.h>
#include <stdbool.h>
#include <stdio.h>

// Temporary counters as a neuron activating sample for clicking a button.
static i32 ButtonOneCount = 0;
static i32 ButtonTwoCount = 0;

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

    // Start the frame
    jui_start_frame();

    // First window
    if (jui_begin("Window One", 156, 128))
    {
      const char* LabelText = "Label";
      jui_label(LabelText, 10, 20);

      char Buffer[15];
      snprintf(Buffer, sizeof(Buffer), "One: %d", ButtonOneCount);
      if (jui_button(Buffer, 10, 35, 90, 32))
      {
        ButtonOneCount++;
      }

      snprintf(Buffer, sizeof(Buffer), "Two: %d", ButtonTwoCount);
      if (jui_button(Buffer, 10, 72, 90, 48))
      {
        ButtonTwoCount++;
      }
    }
    jui_end();

    // Second window
    if (jui_begin("Window Two", 128, 128))
    {
      const char* LabelText = "Some label";
      jui_label(LabelText, 10, 20);

      if (jui_button("Two", 10, 40, 55, 45))
      {
        printf("Hit the button in the second window!\n");
      }
    }
    jui_end();

    // Third window
    if (jui_begin("Window Three", 128, 148))
    {
      const char* LabelText = "Some label";
      jui_label(LabelText, 10, 20);

      if (jui_button("Three", 10, 40, 55, 45))
      {
        printf("Hit the button in the third window!\n");
      }
    }
    jui_end();

    // Render the UI
    jui_render();

    SDL_RenderPresent(Renderer);
  }

  // Teardown jui
  jui_teardown();

  SDL_DestroyRenderer(Renderer);
  SDL_DestroyWindow(Window);
  SDL_Quit();

  return 0;
}
