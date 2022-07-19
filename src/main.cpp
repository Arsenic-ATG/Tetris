/**
This file contains the mail loop
*/

#include <SDL2/SDL.h>
#include <cstdio>


#ifdef __EMSCRIPTEN__
#include "em_app.hpp"

int
emscripten_main ()
{
  unsigned int disp_width = 1280;
  unsigned int disp_height = 720;

  if (!em_app_init (disp_width, disp_height))
    {
      printf ("ERROR - App failed to initialise\n");
      em_app_shut_down ();
      return 1;
    }

  em_app_run ();
  // em_app_shut_down ();

  return 0;
}

#else
#include "app.hpp"
#endif

int
main ()
{
#ifdef __EMSCRIPTEN__
  emscripten_main ();

#else
  bool is_full_screen = false;
  unsigned int disp_width = 1280;
  unsigned int disp_height = 720;

  app application;
  if (!application.init (is_full_screen, disp_width, disp_height))
    {
      printf ("ERROR - App failed to initialise\n");
      application.shut_down ();
      return 1;
    }

  application.run ();
  application.shut_down ();
#endif
  return 0;
}
