#include "em_app.hpp"
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

static bool is_done = false;
static auto start_time = std::chrono::high_resolution_clock::now ();

// helpers
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

static auto
process_input (game_input &input)
{
  // respond to events
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
            case SDLK_SPACE:
              input.m_start = true;
              break;
            case SDLK_LEFT:
              input.m_move_left = true;
              break;
            case SDLK_RIGHT:
              input.m_move_right = true;
              break;
            case SDLK_z:
              input.m_rotate_clockwise = true;
              break;
            case SDLK_x:
              input.m_rotate_anticlockwise = true;
              break;
            case SDLK_UP:
              input.m_hard_drop = true;
              break;
            case SDLK_DOWN:
              input.m_soft_drop = true;
              break;
            case SDLK_p:
              input.m_pause = true;
              break;
            }
        }
    }
}

static void
event_loop_handler ()
{

#ifdef __EMSCRIPTEN__
  if (is_done)
    emscripten_cancel_main_loop ();
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
