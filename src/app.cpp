/** @file app.hpp
 *  @brief function prototypes for application driver
 *
 *  This contains the prototypes for the application driver which is resposible
 *  for initialising, running and shutting down the main application.
 *
 */

#include "app.hpp"
#include "game.hpp"
#include "renderer.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <chrono>
#include <cstdio>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace application;

SDL_Window *g_window;
renderer *g_renderer;
game *g_game;

static bool is_done = false; /**< used to break the main event loop*/

static auto start_time
    = std::chrono::high_resolution_clock::now (); /**< used to calculate fps
                                                     and calculate time for new
                                                     frame to be rendererd */

/** @brief Print SDL version info on stdout
 *
 *  @param Info that is to be printed before version info
 *  @param object of type SDL_version, containg version info
 *  @return Void
 */
static void
print_SDL_version (const char *preamble, const SDL_version &v)
{
  printf ("%s %u.%u.%u\n", preamble, v.major, v.minor, v.patch);
}

static bool
operator== (SDL_version &a, SDL_version &b)
{
  return (a.major == b.major) && (a.minor == b.minor) && (a.patch == b.patch);
}

/** @brief check and initialise application
 *
 *  Initialise application by checking and initialising sdl, sdl windows, sdl
 *  renderer and sdl_ttf
 *
 *  @param width of the logical screen
 *  @param height of the logical screen
 *  @return true if the app was initialized properly, false otherwise
 */
bool
application::init_app (const unsigned int width, const unsigned int height)
{
  if (SDL_Init (SDL_INIT_VIDEO) != 0)
    {
      fprintf (stderr, "SDL failed to initialise: %s\n", SDL_GetError ());
      return false;
    }

  printf ("SDL initialised\n");

  SDL_version compiled_version;
  SDL_version linked_version;
  SDL_VERSION (&compiled_version);
  SDL_GetVersion (&linked_version);
  print_SDL_version ("Compiled against SDL version", compiled_version);
  print_SDL_version ("Linking against SDL version", linked_version);
  SDL_assert_release ((compiled_version == linked_version));

  int num_displays = SDL_GetNumVideoDisplays ();
  printf ("%d video displays\n", num_displays);
  for (int i = 0; i < num_displays; ++i)
    {
      SDL_DisplayMode display_mode;
      if (SDL_GetCurrentDisplayMode (i, &display_mode) != 0)
        {
          fprintf (stderr,
                   "Failed to get display mode for video display %d: %s", i,
                   SDL_GetError ());
          continue;
        }

      printf ("Display %d: w=%d, h=%d refresh_rate=%d\n", i, display_mode.w,
              display_mode.h, display_mode.refresh_rate);
    }

  const char *title = "Tetris";
  g_window = SDL_CreateWindow (title, SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, width, height,
                               SDL_WINDOW_SHOWN);

  if (!g_window)
    {
      printf ("Failed to create SDL window: %s\n", SDL_GetError ());
      return false;
    }

  // SDL2_ttf

  if (TTF_Init () == -1)
    {
      fprintf (stderr, "Failed to initialise SDL2_ttf: %s\n", TTF_GetError ());
      return false;
    }

  printf ("SDL_ttf initialised\n");

  SDL_TTF_VERSION (&compiled_version);
  const SDL_version *linked_version_ptr = TTF_Linked_Version ();
  print_SDL_version ("Compiled against SDL_ttf version", compiled_version);
  print_SDL_version ("Linking against SDL_ttf version", *linked_version_ptr);

  unsigned int logicalWidth = 1280;
  unsigned int logicalHeight = 720;
  g_renderer = new renderer (*g_window, logicalWidth, logicalHeight);

  g_game = new game ();

  if (!g_game->init_game ())
    {
      fprintf (stderr, "ERROR - Game failed to initialise\n");
      return false;
    }

  return true;
}

/** @brief process the keypress of the user
 *
 *  process and translate the keypresses by the user to corresponding input to
 *  the game
 *
 *  current inputs are as follows:
 *
 *  esc/q        -> exit game
 *  enter        -> start game
 *  left_arrow   -> move left
 *  right_arrow  -> move right
 *  z/up_arrow   -> rotate clockwise
 *  x/ctrl       -> rotate counter clockwise
 *  space        -> hard drop
 *  down_arrow   -> soft drop
 *  p            -> pause game
 *
 *  @param reference of game input object that needs to be updated with
 *  processed info
 *  @return Void
 */
static void
process_input (game_input &input)
{
  SDL_Event event;
  while (SDL_PollEvent (&event))
    {
      if (event.type == SDL_QUIT)
        {
          is_done = true;
        }

      // quit if escape pressed
      if (event.type == SDL_KEYDOWN)
        {
          switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
            case SDLK_q:
              is_done = true;
              break;
            case SDLK_RETURN:
              input.m_start = true;
              break;
            case SDLK_LEFT:
              input.m_move_left = true;
              break;
            case SDLK_RIGHT:
              input.m_move_right = true;
              break;
            case SDLK_z:
            case SDLK_UP:
              input.m_rotate_clockwise = true;
              break;
            case SDLK_x:
            case SDLK_LCTRL:
              input.m_rotate_anticlockwise = true;
              break;
            case SDLK_DOWN:
              input.m_soft_drop = true;
              break;
            case SDLK_p:
              input.m_pause = true;
              break;
            case SDLK_r:
              input.m_reset = true;
              break;
            case SDLK_SPACE:
              input.m_hard_drop = true;
              break;
            }
        }
    }
}

/** @brief handle the main event loop of the entire application
 *
 *  This function would either be ran in an infilite loop (until is_done is
 *  false) or would be passed to emscripten_set_main_loop () to run emulate
 *  main event loop on web-browser
 *
 *  @return Void
 */
static void
event_loop_handler ()
{

#ifdef __EMSCRIPTEN__
  if (is_done)
    {
      emscripten_cancel_main_loop ();
      shut_down_app ();
    }

#endif

  game_input input = {};
  process_input (input);

  auto current_time = std::chrono::high_resolution_clock::now ();
  auto delta_time = current_time - start_time;
  auto delta_time_microsec
      = std::chrono::duration_cast<std::chrono::microseconds> (delta_time);
  auto delta_time_seconds
      = 0.000001f * static_cast<float> (delta_time_microsec.count ());
  start_time = current_time;

  g_game->update (input, delta_time_seconds);

  g_renderer->clear ();
  g_game->draw (*g_renderer);
  g_renderer->present ();
}

/** @brief Run the application main loop
 *
 *  Initialize the start time and start the main event loop of the application
 *  which is an infinite loop in case of native build, emscripten main loop in
 *  case of emscripten build.
 *
 *  we can't use infinite loop on web-brower as control never returns to
 *  browser while the app renderes and app appears to be hanged causing the
 *  browser to notify the user and eliminte the program.
 *
 *  check the following link for more info to why this is the case:
 *  https://emscripten.org/docs/porting/emscripten-runtime-environment.html#browser-main-loop
 *
 *  @return Void
 */
void
application::run_app ()
{
  start_time = std::chrono::high_resolution_clock::now ();
  is_done = false;

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop (event_loop_handler, -1, 0);
#else
  while (!is_done)
    {
      event_loop_handler ();
    }
#endif
}

void
application::shut_down_app ()
{
  if (g_game)
    {
      g_game->shutdown ();
      delete g_game;
      g_game = nullptr;
    }

  delete g_renderer;
  g_renderer = nullptr;

  TTF_Quit ();
  SDL_DestroyWindow (g_window);
  SDL_Quit ();
}
