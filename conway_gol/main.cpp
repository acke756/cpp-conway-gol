#include <string>

#include <SDL2/SDL.h>

#include <conway_gol/gol.hpp>

using namespace conway_gol;

typedef struct pixel_rgb24 {
  uint8_t r, g, b;
} pixel_rgb24_t;

class GolView {
  public:
    GolView() = delete;

    GolView(SDL_Renderer* renderer, const Gol& gol):
        texture_(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
          SDL_TEXTUREACCESS_STREAMING, gol.width(), gol.height())),
        gol_(gol) {
    }

    GolView(const GolView&) = delete;
    GolView(GolView&& other):
        texture_(other.texture_),
        gol_(other.gol_) {
      other.texture_ = NULL;
    }

    GolView& operator=(GolView&) = delete;
    GolView& operator=(GolView&&) = delete;

    ~GolView() {
      SDL_DestroyTexture(texture_);
    }

    explicit operator bool() const noexcept {
      return texture_;
    }

    int draw(SDL_Renderer* renderer, const SDL_Rect* dstrect) {
      pixel_rgb24_t* pixels;
      int pitch;

      if (SDL_LockTexture(texture_, NULL, (void**) &pixels, &pitch) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s", SDL_GetError());
        return EXIT_FAILURE;
      }

      static constexpr pixel_rgb24_t alive = { 0xFF, 0xFF, 0xFF };
      static constexpr pixel_rgb24_t dead = {0};

      for (Gol::size_type row = 0; row < gol_.height(); row++) {
        for (Gol::size_type column = 0; column < gol_.width(); column++) {
          size_t pixel_index = (row * gol_.width() + column);
          pixels[pixel_index] = gol_.at(column, row) ? alive : dead;
        }
      }

      SDL_UnlockTexture(texture_);

      if (SDL_RenderCopy(renderer, texture_, NULL, dstrect) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't copy texture to renderer: %s", SDL_GetError());
        return EXIT_FAILURE;
      }

      return EXIT_SUCCESS;
    }

  private:
    SDL_Texture* texture_;
    const Gol& gol_;
};

class GolController {
  public:
    GolController() = delete;
    GolController(Gol& gol):
        gol_(gol) {
    }

    void handle_event(const SDL_KeyboardEvent& event) {
      if (event.type == SDL_KEYUP) {
        return;
      }

      switch (event.keysym.sym) {
        case SDLK_RETURN:
          gol_.update();
          break;

        case SDLK_w:
          if (event.keysym.mod & KMOD_CTRL
              && !(event.keysym.mod & (KMOD_SHIFT | KMOD_ALT))) {
            push_quit_event_(event.timestamp);
          }
          break;
      }
    }

  private:
    Gol& gol_;

    void push_quit_event_(Uint32 timestamp) {
      SDL_Event event;
      event.type = SDL_QUIT;
      event.quit.timestamp = timestamp;

      SDL_PushEvent(&event);
    }
};

int on_create_renderer(SDL_Renderer* renderer) {
  SDL_Event event;
  conway_gol::Gol gol(80, 50);
  GolController controller(gol);

  GolView gol_view(renderer, gol);
  if (!gol_view) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create gol_view: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  for (;;) {
    if (gol_view.draw(renderer, NULL) < 0) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't copy texture to renderer: %s", SDL_GetError());
      return EXIT_FAILURE;
    }

    SDL_RenderPresent(renderer);

    if (SDL_WaitEvent(&event) == 0) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while waiting for event: %s", SDL_GetError());
      return EXIT_FAILURE;
    }

    if (event.type == SDL_QUIT) {
      return EXIT_SUCCESS;
    }

    if (event.type == SDL_KEYUP || event.type == SDL_KEYDOWN) {
      controller.handle_event(event.key);
    }
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
