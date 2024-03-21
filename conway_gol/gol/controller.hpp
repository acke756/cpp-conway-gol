#pragma once

#include <conway_gol/gol/model.hpp>
#include <conway_gol/gol/view.hpp>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

namespace conway_gol {
  class GolController {
    public:
      GolController() = delete;
      GolController(Gol& gol, GolView&& gol_view);

      int on_program_start(SDL_Renderer* renderer);
      int handle_event(SDL_Renderer* renderer, const SDL_Event& event);
      int handle_event(SDL_Renderer* renderer, const SDL_KeyboardEvent& event);
      int handle_event(SDL_Renderer* renderer, const SDL_WindowEvent& event);

    private:
      Gol& gol_;
      GolView gol_view_;

      void push_quit_event_(Uint32 timestamp);
  };

} // namespace conway_gol
