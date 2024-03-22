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
    int err = SDL_RenderCopyF(renderer_, texture_.get(), NULL, &draw_rect_);
    if (err) {
      return err;
    }

    err = draw_highlight_();
    if (err) {
      return err;
    }

    return 0;
  }

  int GolView::update() {
    Uint8* pixels;
    int pitch;
    int err;

    err = SDL_LockTexture(texture_.get(), NULL,
        (void**) &pixels, &pitch);
    if (err) {
      return err;
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
    return 0;
  }

  std::optional<Gol::coordinate> GolView::cell_at(
      Sint32 window_x, Sint32 window_y) {
    float x = (static_cast<float>(window_x) - draw_rect_.x) / pixels_per_cell_;
    float y = (static_cast<float>(window_y) - draw_rect_.y) / pixels_per_cell_;

    if (x < 0 || y < 0) {
      x = -1.0f;
      y = -1.0f;
    }

    std::optional<Gol::coordinate> retval;
    Gol::coordinate c {
      .column = static_cast<Gol::size_type>(x),
      .row = static_cast<Gol::size_type>(y),
    };

    if (gol_.has_data_at(c)) {
      retval = c;
    }

    return retval;
  }

  void GolView::highlight(const std::optional<Gol::coordinate>& cell) noexcept {
    highlight_ = cell;
  }

  const std::optional<Gol::coordinate>& GolView::highlight() const noexcept {
    return highlight_;
  }

  // --- Private member functions ---

  int GolView::draw_highlight_() {
    if (!highlight_) {
      return 0;
    }

    int err;

    err = SDL_SetRenderDrawColor(renderer_, 0xFF, 0, 0, 0xFF);
    if (err) {
      return err;
    }

    SDL_FRect rect = rect_of_(*highlight_);
    err = SDL_RenderDrawRectF(renderer_, &rect);
    if (err) {
      return err;
    }

    return 0;
  }

  SDL_FRect GolView::rect_of_(const Gol::coordinate& cell) {
    return {
      .x = (pixels_per_cell_ * cell.column) - draw_rect_.x,
      .y = (pixels_per_cell_ * cell.row) - draw_rect_.y,
      .w = pixels_per_cell_,
      .h = pixels_per_cell_,
    };
  }

} // namespace conway_gol
