#pragma once

#include <SDL2/SDL_pixels.h>

#include <memory>

namespace conway_gol {
  class PixelFormatDeleter {
    public:
      void operator()(SDL_PixelFormat* format) noexcept {
        SDL_FreeFormat(format);
      }
  };

  using unique_pixel_format_ptr = typename
    std::unique_ptr<SDL_PixelFormat, PixelFormatDeleter>;

} // namespace conway_gol
