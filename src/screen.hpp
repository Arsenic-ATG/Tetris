#ifndef SCREEN_H
#define SCREEN_H

#ifndef LINUX
#include "SDL/include/SDL.h"
#include "SDL/SDL_GfxPrimitives/sdl_gfxprimitives.h"
#else
#include <SDL/SDL.h>
#include "SDL/SDL_GfxPrimitives/sdl_gfxprimitives.h"
#endif

#include "point.hpp"

// TODO: put this stuff inside some namespace (namespace UI)

enum class color {black, red, green, blue, cyan, magenta, yellow, white, color_count};

class screen
{
public:
  screen ();

  auto draw_rectangle (point p1, point p2, color fill_color) -> void;
  auto clear () -> void;
  auto get_height () -> int;
  auto get_key () -> int;
  auto poll_key () -> int;
  auto update_screen () -> void;
  auto is_key_pressed (int key) -> int;

  private:
  auto init_graph () -> int;

};

#endif /* SCREEN_H */
