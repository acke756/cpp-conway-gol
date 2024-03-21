#pragma once

#include <SDL2/SDL_render.h>

#include <memory>

namespace conway_gol {
  class TextureDeleter {
    public:
      void operator()(SDL_Texture* texture) noexcept {
        SDL_DestroyTexture(texture);
      }
  };

  using unique_texture_ptr = typename
    std::unique_ptr<SDL_Texture, TextureDeleter>;

} // namespace conway_gol
