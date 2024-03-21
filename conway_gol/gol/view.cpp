#include <conway_gol/gol/view.hpp>

namespace conway_gol {

  // --- Public member functions ---

  GolView::GolView(SDL_Renderer* renderer, const Gol& gol, const SDL_Rect& draw_rect):
      pixel_format_(SDL_AllocFormat(SDL_PIXELFORMAT_RGB24)),
      texture_(SDL_CreateTexture(renderer, pixel_format_->format,
            SDL_TEXTUREACCESS_STREAMING, gol.width(), gol.height())),
      gol_(gol),
      pixels_per_cell_(8.0f),
      draw_rect_({
        .w = pixels_per_cell_ * gol_.width(),
        .h = pixels_per_cell_ * gol_.height(),
      }) {
  }

  int GolView::draw(SDL_Renderer* renderer) {
    Uint8* pixels;
    int pitch;

    int lock_result = SDL_LockTexture(texture_.get(), NULL,
        (void**) &pixels, &pitch);
    if (lock_result < 0) {
      return lock_result;
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

    return SDL_RenderCopyF(renderer, texture_.get(), NULL, &draw_rect_);
  }

} // namespace conway_gol
