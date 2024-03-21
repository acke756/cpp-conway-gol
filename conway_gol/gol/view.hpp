#pragma once

#include <conway_gol/gol/model.hpp>

#include <conway_gol/sdl_helpers/pixel_format.hpp>
#include <conway_gol/sdl_helpers/texture.hpp>

#include <SDL2/SDL_render.h>

namespace conway_gol {
  class GolView {
    public:
      GolView() = delete;
      GolView(SDL_Renderer* renderer, const Gol& gol, SDL_Rect draw_rect);

      GolView(const GolView&) = delete;
      GolView(GolView&& other) = default;

      GolView& operator=(GolView&) = delete;
      GolView& operator=(GolView&&) = delete;

      ~GolView() = default;

      explicit operator bool() const noexcept {
        return static_cast<bool>(texture_);
      }

      inline void set_draw_rect(const SDL_Rect& draw_rect) {
        draw_rect_ = draw_rect;
      }

      int draw(SDL_Renderer* renderer);

    private:
      unique_pixel_format_ptr pixel_format_;
      unique_texture_ptr texture_;
      const Gol& gol_;
      SDL_Rect draw_rect_;
  };

} // namespace conway_gol
