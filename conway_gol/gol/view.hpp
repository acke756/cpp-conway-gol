#pragma once

#include <conway_gol/gol/model.hpp>

#include <conway_gol/sdl_helpers/pixel_format.hpp>
#include <conway_gol/sdl_helpers/texture.hpp>

#include <SDL2/SDL_render.h>

namespace conway_gol {
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

      void set_draw_rect(const SDL_Rect& draw_rect) {
        draw_rect_ = draw_rect;
      }

    private:
      unique_pixel_format_ptr pixel_format_;
      unique_texture_ptr texture_;
      const Gol& gol_;
      SDL_Rect draw_rect_;
  };

} // namespace conway_gol
