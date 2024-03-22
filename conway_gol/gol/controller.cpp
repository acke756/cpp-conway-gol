#include <conway_gol/gol/controller.hpp>

#include <optional>

namespace conway_gol {

  // --- Public member functions ---

  GolController::GolController(IGol& gol, GolView&& gol_view):
      gol_(gol),
      gol_view_(std::move(gol_view)) {
  }

  int GolController::on_program_start() {
    int err;
    
    err = gol_view_.update();
    if (err) {
      return err;
    }

    err = gol_view_.draw();
    if (err) {
      return err;
    }

    return 0;
  }

  int GolController::handle_event(const SDL_Event& event) {
    switch (event.type) {
      case SDL_KEYDOWN:
        return handle_event(event.key);

      case SDL_MOUSEBUTTONDOWN:
        return handle_event(event.button);

      case SDL_MOUSEMOTION:
        return handle_event(event.motion);
        
      case SDL_WINDOWEVENT:
        return handle_event(event.window);

      default:
        return 0;
    }
  }

  int GolController::handle_event(const SDL_KeyboardEvent& event) {
    if (event.type == SDL_KEYUP) {
      return 0;
    }

    int err;
    switch (event.keysym.sym) {
      case SDLK_RETURN:
        gol_.update();

        err = gol_view_.update();
        if (err) {
          return err;
        }

        err = gol_view_.draw();
        if (err) {
          return err;
        }

        return 0;

      case SDLK_w:
        if (event.keysym.mod & KMOD_CTRL
            && !(event.keysym.mod & (KMOD_SHIFT | KMOD_ALT))) {
          push_quit_event_(event.timestamp);
        }
        break;
    }

    return 0;
  }

  int GolController::handle_event(const SDL_MouseButtonEvent& event) {
    int err;
    std::optional<IGol::coordinate> c;

    switch (event.button) {
      case SDL_BUTTON_LEFT:
        c = gol_view_.cell_at(event.x, event.y);
        if (!c) {
          return 0;
        }

        gol_.at(*c) = !gol_.at(*c);

        err = gol_view_.update();
        if (err) {
          return err;
        }

        err = gol_view_.draw();
        if (err) {
          return err;
        }

        return 0;

      default:
        return 0;
    }
  }

  int GolController::handle_event(const SDL_MouseMotionEvent& event) {
    std::optional<IGol::coordinate> cell = gol_view_.cell_at(event.x, event.y);
    if (cell == gol_view_.highlight()) {
      return 0;
    }

    gol_view_.highlight(cell);
    return gol_view_.draw();
  }

  int GolController::handle_event(const SDL_WindowEvent& event) {
    if (event.event != SDL_WINDOWEVENT_RESIZED) {
      return 0;
    }

    return gol_view_.draw();
  }

  // --- Private member functions ---

  void GolController::push_quit_event_(Uint32 timestamp) {
    SDL_Event event;
    event.type = SDL_QUIT;
    event.quit.timestamp = timestamp;

    SDL_PushEvent(&event);
  }

} // namespace conway_gol
