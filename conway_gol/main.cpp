#include <memory>
#include <string>

#include <SDL2/SDL.h>

#include <conway_gol/gol/gol.hpp>

using namespace conway_gol;

int on_create_renderer(SDL_Renderer* renderer) {
  SDL_Event event;
  conway_gol::Gol gol(80, 50);

  GolView gol_view(renderer, gol);
  if (!gol_view) {
    return EXIT_FAILURE;
  }

  GolController controller(gol, std::move(gol_view));
  if (controller.on_program_start() < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GolController error on program start: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  for (;;) {

    if (SDL_WaitEvent(&event) == 0) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while waiting for event: %s", SDL_GetError());
      return EXIT_FAILURE;
    }

    if (event.type == SDL_QUIT) {
      return EXIT_SUCCESS;
    }

    if (controller.handle_event(event) < 0) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GolController error while handling event: %s", SDL_GetError());
      return EXIT_FAILURE;
    }

    SDL_RenderPresent(renderer);
  }

  return EXIT_SUCCESS;
}

int on_create_window(SDL_Window* window) {
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  int retval = on_create_renderer(renderer);

  SDL_DestroyRenderer(renderer);
  return retval;
}

int on_sdl_init() {
  SDL_Window* window = SDL_CreateWindow("Game of life",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      640, 480, SDL_WINDOW_RESIZABLE);

  if (window == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  int retval = on_create_window(window);

  SDL_DestroyWindow(window);
  return retval;
}

int main(int argc, char** argv) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  int retval = on_sdl_init();

  SDL_Quit();
  return retval;
}
