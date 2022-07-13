/**
This file contains the mail loop
*/

#include "app.hpp"
#include <SDL2/SDL.h>
#include <cstdio>

int
main ()
{
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

  return 0;
}
