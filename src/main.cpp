/**
This file contains the mail loop
*/

#include "app.hpp"
#include <SDL2/SDL.h>
#include <cstdio>

int
main ()
{
  constexpr unsigned int disp_width = 1280;
  constexpr unsigned int disp_height = 720;

  if (!application::init_app (disp_width, disp_height))
    {
      printf ("ERROR - App failed to initialise\n");
      application::shut_down_app ();
      return 1;
    }
  application::run_app ();
  return 0;
}
