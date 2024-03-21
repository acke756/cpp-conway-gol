#include <conway_gol/gol/controller.hpp>

namespace conway_gol {

  // --- Public member functions ---

  GolController::GolController(Gol& gol, GolView&& gol_view):
      gol_(gol),
      gol_view_(std::move(gol_view)) {
  }

  int GolController::on_program_start(SDL_Renderer* renderer) {
    return gol_view_.draw(renderer);
  }

  int GolController::handle_event(SDL_Renderer* renderer,
      const SDL_Event& event) {
    switch (event.type) {
      case SDL_KEYDOWN:
        return handle_event(renderer, event.key);
        
      case SDL_WINDOWEVENT:
        return handle_event(renderer, event.window);

      default:
        return 0;
    }
  }

  int GolController::handle_event(SDL_Renderer* renderer,
      const SDL_KeyboardEvent& event) {
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

  int GolController::handle_event(SDL_Renderer* renderer,
      const SDL_WindowEvent& event) {
    if (event.event != SDL_WINDOWEVENT_RESIZED) {
      return 0;
    }

    return gol_view_.draw(renderer);
  }

  // --- Private member functions ---

  void GolController::push_quit_event_(Uint32 timestamp) {
    SDL_Event event;
    event.type = SDL_QUIT;
    event.quit.timestamp = timestamp;

    SDL_PushEvent(&event);
  }

} // namespace conway_gol
