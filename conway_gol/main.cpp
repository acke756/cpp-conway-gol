#include <iostream>
#include <string>

#include <SDL2/SDL.h>

#include <conway_gol/gol.hpp>

void hide_cursor(std::ostream& out) {
  out << "\033[?25l";
}

void show_cursor(std::ostream& out) {
  out << "\033[?25h";
}

void clear_screen(std::ostream& out) {
  out << "\033[;H\033[2J";
}

void enable_alt_buffer(std::ostream& out) {
  out << "\033[?1049h";
}

void disable_alt_buffer(std::ostream& out) {
  out << "\033[?1049l";
}

int on_create_renderer(SDL_Renderer* renderer) {
  SDL_Event event;
  for (;;) {
    if (SDL_WaitEvent(&event) == 0) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while waiting for event: %s", SDL_GetError());
      return EXIT_FAILURE;
    }

    if (event.type == SDL_QUIT) {
      return EXIT_SUCCESS;
    }

    SDL_SetRenderDrawColor(renderer, 0x0F, 0x0F, 0x0F, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }
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

/*
int main(int argc, char** argv) {
  conway_gol::Gol gol(80, 50);
  auto& in = std::cin;
  auto& out = std::cout;

  enable_alt_buffer(out);
  hide_cursor(out);
  out.flush();

  std::string line;
  while (in.good()) {
    clear_screen(out);
    out << gol;
    out.flush();

    gol.update();
    std::getline(in, line);
  }

  show_cursor(out);
  disable_alt_buffer(out);
  out.flush();

  return EXIT_SUCCESS;
}
*/
