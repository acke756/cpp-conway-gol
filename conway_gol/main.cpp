#include <memory>
#include <string>

#include <SDL2/SDL.h>

#include <conway_gol/gol.hpp>

using namespace conway_gol;

class PixelFormatDeleter {
  public:
    void operator()(SDL_PixelFormat* format) noexcept {
      SDL_FreeFormat(format);
    }
};

class TextureDeleter {
  public:
    void operator()(SDL_Texture* texture) noexcept {
      SDL_DestroyTexture(texture);
    }
};

typedef std::unique_ptr<SDL_PixelFormat, PixelFormatDeleter> unique_pixel_format_ptr;
typedef std::unique_ptr<SDL_Texture, TextureDeleter> unique_texture_ptr;

class GolView {
  public:
    GolView() = delete;

    GolView(SDL_Renderer* renderer, const Gol& gol, SDL_Rect draw_rect):
        pixel_format_(SDL_AllocFormat(SDL_PIXELFORMAT_RGB24)),
        texture_(SDL_CreateTexture(renderer, pixel_format_->format,
          SDL_TEXTUREACCESS_STREAMING, gol.width(), gol.height())),
        gol_(gol),
        draw_rect_(draw_rect) {
    }

    GolView(const GolView&) = delete;
    GolView(GolView&& other) = default;

    GolView& operator=(GolView&) = delete;
    GolView& operator=(GolView&&) = delete;

    ~GolView() = default;

    explicit operator bool() const noexcept {
      return static_cast<bool>(texture_);
    }

    int draw(SDL_Renderer* renderer) {
      Uint8* pixels;
      int pitch;

      if (SDL_LockTexture(texture_.get(), NULL, (void**) &pixels, &pitch) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s", SDL_GetError());
        return EXIT_FAILURE;
      }

      static constexpr SDL_Color alive_color = { 0xFF, 0xFF, 0xFF, 0xFF };
      static constexpr SDL_Color dead_color = { 0, 0, 0, 0xFF };

      for (Gol::size_type row = 0; row < gol_.height(); row++) {
        for (Gol::size_type column = 0; column < gol_.width(); column++) {
          size_t pixel_index = (row * gol_.width() + column) * pixel_format_->BytesPerPixel;
          const SDL_Color& color = gol_.at(column, row) ? alive_color : dead_color;
          *((Uint32*) (pixels + pixel_index)) =
            SDL_MapRGBA(pixel_format_.get(), color.r, color.g, color.b, color.a);
        }
      }

      SDL_UnlockTexture(texture_.get());

      if (SDL_RenderCopy(renderer, texture_.get(), NULL, &draw_rect_) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't copy texture to renderer: %s", SDL_GetError());
        return EXIT_FAILURE;
      }

      return EXIT_SUCCESS;
    }

  private:
    unique_pixel_format_ptr pixel_format_;
    unique_texture_ptr texture_;
    const Gol& gol_;
    SDL_Rect draw_rect_;
};

class GolController {
  public:
    GolController() = delete;
    GolController(Gol& gol, GolView&& gol_view):
        gol_(gol),
        gol_view_(std::move(gol_view)) {
    }

    int on_program_start(SDL_Renderer* renderer) {
      return gol_view_.draw(renderer);
    }

    int handle_event(SDL_Renderer* renderer, const SDL_KeyboardEvent& event) {
      if (event.type == SDL_KEYUP) {
        return 0;
      }

      switch (event.keysym.sym) {
        case SDLK_RETURN:
          gol_.update();
          return gol_view_.draw(renderer);

        case SDLK_w:
          if (event.keysym.mod & KMOD_CTRL
              && !(event.keysym.mod & (KMOD_SHIFT | KMOD_ALT))) {
            push_quit_event_(event.timestamp);
          }
          break;
      }

      return 0;
    }

  private:
    Gol& gol_;
    GolView gol_view_;

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

  SDL_Rect gol_draw_rect{0};
  if (SDL_GetRendererOutputSize(renderer, &gol_draw_rect.w, &gol_draw_rect.h) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't get renderer size: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  GolView gol_view(renderer, gol, gol_draw_rect);
  if (!gol_view) {
    return EXIT_FAILURE;
  }

  GolController controller(gol, std::move(gol_view));
  if (controller.on_program_start(renderer) < 0) {
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

    if (event.type == SDL_KEYUP || event.type == SDL_KEYDOWN) {
      if (controller.handle_event(renderer, event.key) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GolController error while handling event: %s", SDL_GetError());
        return EXIT_FAILURE;
      }
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
