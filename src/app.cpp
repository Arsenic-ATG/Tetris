#include "app.hpp"
#include "game.hpp"
#include "renderer.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <chrono>
#include <cstdio>

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

// class app
app::app () : m_window (nullptr), m_renderer (nullptr), m_game (nullptr) {}

auto
app::init (bool is_full_screen, unsigned int width, unsigned int height)
    -> bool
{
  if (SDL_Init (SDL_INIT_EVERYTHING) != 0)
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

  const char *title = "SDL Window";
  if (is_full_screen)
    {
      m_window = SDL_CreateWindow (title, SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, 0, 0,
                                   SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
  else
    {
      m_window = SDL_CreateWindow (title, SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, width, height,
                                   SDL_WINDOW_SHOWN);
    }

  if (!m_window)
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
  m_renderer = new renderer (*m_window, logicalWidth, logicalHeight);

  m_game = new game ();

  if (!m_game->init_game ())
    {
      fprintf (stderr, "ERROR - Game failed to initialise\n");
      return false;
    }

  return true;
}

auto
app::shut_down () -> void
{
  if (m_game)
    {
      m_game->shutdown ();
      delete m_game;
      m_game = nullptr;
    }

  delete m_renderer;
  m_renderer = nullptr;

  TTF_Quit ();
  SDL_DestroyWindow (m_window);
  SDL_Quit ();
}

auto
app::run () -> void
{
  Uint32 last_time_ms = SDL_GetTicks (); // only ms accuracy
  auto last_time
      = std::chrono::high_resolution_clock::now (); // high-precision time

  bool is_done = false;
  while (!is_done)
    {
      game_input input = {};

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

      Uint32 current_time_ms = SDL_GetTicks ();
      // Uint32 delta_time_ms = current_time_ms - last_time_ms;
      last_time_ms = current_time_ms;

      auto currentTime
          = std::chrono::high_resolution_clock::now (); // high-precision time
      auto deltaTime = currentTime - last_time;
      std::chrono::microseconds delta_time_microsec
          = std::chrono::duration_cast<std::chrono::microseconds> (deltaTime);
      auto deltaTimeSeconds
          = 0.000001f * static_cast<float> (delta_time_microsec.count ());
      last_time = currentTime;

      m_game->update (input, deltaTimeSeconds);

      m_renderer->clear ();
      m_game->draw (*m_renderer);
      m_renderer->present ();
    }
}
