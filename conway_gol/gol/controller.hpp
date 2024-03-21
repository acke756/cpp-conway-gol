#pragma once

#include <conway_gol/gol/model.hpp>
#include <conway_gol/gol/view.hpp>

namespace conway_gol {
  class GolController {
    public:
      GolController() = delete;
      GolController(Gol& gol, GolView&& gol_view):
        gol_(gol),
        gol_view_(std::move(gol_view)) {
        }

      int on_program_start(SDL_Renderer* renderer) {
        return gol_view_.draw(renderer);
      }

      int handle_event(SDL_Renderer* renderer, const SDL_Event& event) {
        switch (event.type) {
          case SDL_KEYDOWN:
            return handle_event(renderer, event.key);

          case SDL_WINDOWEVENT:
            return handle_event(renderer, event.window);

          default:
            return 0;
        }
      }

      int handle_event(SDL_Renderer* renderer, const SDL_KeyboardEvent& event) {
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

      int handle_event(SDL_Renderer* renderer, const SDL_WindowEvent& event) {
        if (event.event != SDL_WINDOWEVENT_RESIZED) {
          return 0;
        }

        SDL_Rect view_rect{0};
        int retval = SDL_GetRendererOutputSize(renderer, &view_rect.w, &view_rect.h);
        if (retval < 0) {
          return retval;
        }

        gol_view_.set_draw_rect(view_rect);
        return gol_view_.draw(renderer);
      }

    private:
      Gol& gol_;
      GolView gol_view_;

      void push_quit_event_(Uint32 timestamp) {
        SDL_Event event;
        event.type = SDL_QUIT;
        event.quit.timestamp = timestamp;

        SDL_PushEvent(&event);
      }
  };

} // namespace conway_gol
