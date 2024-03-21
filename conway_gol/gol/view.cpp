#include <conway_gol/gol/view.hpp>

namespace conway_gol {

  // --- Public member functions ---

  GolView::GolView(SDL_Renderer* renderer, const Gol& gol):
      renderer_(renderer),
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

  int GolView::draw() {
    Uint8* pixels;
    int pitch;

    int lock_result = SDL_LockTexture(texture_.get(), NULL,
        (void**) &pixels, &pitch);
    if (lock_result < 0) {
      return lock_result;
    }

    static constexpr SDL_Color alive_color = { 0xFF, 0xFF, 0xFF, 0xFF };
    static constexpr SDL_Color dead_color = { 0, 0, 0, 0xFF };

    Gol::coordinate c;
    for (c.row = 0; c.row < gol_.height(); c.row++) {
      for (c.column = 0; c.column < gol_.width(); c.column++) {
        size_t pixel_index = (c.row * gol_.width() + c.column) * pixel_format_->BytesPerPixel;
        const SDL_Color& color = gol_.at(c) ? alive_color : dead_color;
        *((Uint32*) (pixels + pixel_index)) =
          SDL_MapRGBA(pixel_format_.get(), color.r, color.g, color.b, color.a);
      }
    }

    SDL_UnlockTexture(texture_.get());

    return SDL_RenderCopyF(renderer_, texture_.get(), NULL, &draw_rect_);
  }

  bool GolView::cell_at(Sint32 window_x, Sint32 window_y, Gol::coordinate& c) {
    float x = (static_cast<float>(window_x) - draw_rect_.x) / pixels_per_cell_;
    float y = (static_cast<float>(window_y) - draw_rect_.y) / pixels_per_cell_;

    if (x < 0 || y < 0) {
      return false;
    }

    c.column = static_cast<Gol::size_type>(x);
    c.row = static_cast<Gol::size_type>(y);
    return c.column < gol_.width() && c.row < gol_.height();
  }

} // namespace conway_gol
