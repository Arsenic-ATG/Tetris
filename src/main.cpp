/** @file main.cpp
 *  @brief toplevel entry point of the program
 *
 *  This contains the toplevel main fucntion of the program
 */

#include "app.hpp"
#include <SDL2/SDL.h>
#include <cstdio>

/** @brief entry point of the program
 *
 *  Try to initialise application with provided width and height, and run the
 *  application on successfull initialisation
 *
 *  @return 0 in case of successfull execution, any other number otherwise
 */
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
