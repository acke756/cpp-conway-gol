#pragma once

#include <conway_gol/gol/model.hpp>

#include <conway_gol/sdl_helpers/pixel_format.hpp>
#include <conway_gol/sdl_helpers/texture.hpp>

#include <SDL2/SDL_render.h>

#include <optional>

namespace conway_gol {
  class GolView {
    public:
      GolView() = delete;
      GolView(SDL_Renderer* renderer, const IGol& gol);

      GolView(const GolView&) = delete;
      GolView(GolView&& other) = default;

      GolView& operator=(GolView&) = delete;
      GolView& operator=(GolView&&) = delete;

      ~GolView() = default;

      explicit operator bool() const noexcept {
        return static_cast<bool>(texture_);
      }

      int draw();
      int update();

      std::optional<IGol::coordinate> cell_at(Sint32 window_x, Sint32 window_y);

      void highlight(const std::optional<IGol::coordinate>& cell) noexcept;
      const std::optional<IGol::coordinate>& highlight() const noexcept;

    private:
      SDL_Renderer* renderer_;
      unique_pixel_format_ptr pixel_format_;
      unique_texture_ptr texture_;
      const IGol& gol_;
      float pixels_per_cell_;
      SDL_FRect draw_rect_;
      std::optional<IGol::coordinate> highlight_;

      int draw_highlight_();

      SDL_FRect rect_of_(const IGol::coordinate& cell);
  };

} // namespace conway_gol
