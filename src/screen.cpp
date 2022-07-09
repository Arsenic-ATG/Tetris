#include "screen.hpp"
#include <memory>
#include <unordered_map>

static SDL_Surface *window;

static std::unordered_map<color, Uint32> color_hex
    = { { color::black, 0x000000ff },  { color::red, 0xff0000ff },
        { color::green, 0x00ff00ff },  { color::blue, 0x0000ffff },
        { color::cyan, 0x00ffffff },   { color::magenta, 0xff00ffff },
        { color::yellow, 0xffff00ff }, { color::white, 0xffffffff } };

screen::screen () { init_graph (); }

auto
screen::draw_rectangle (point p1, point p2, color fill_color) -> void
{
  boxColor (window, p1.m_x, p1.m_y, p2.m_x, p2.m_y - 1, color_hex[fill_color]);
}

auto
screen::clear () -> void
{
  boxColor (window, 0, 0, window->w - 1, window->h - 1,
            color_hex[color::black]);
}

auto
screen::get_height () -> int
{
  return window->h;
}

auto
screen::get_key () -> int
{
  SDL_Event event;
  while (true)
    {
      SDL_WaitEvent (&event);
      if (event.type == SDL_KEYDOWN)
        break;
      if (event.type == SDL_QUIT)
        exit (3);
    }
  return event.key.keysym.sym;
}

auto
screen::poll_key () -> int
{
  SDL_Event event;
  while (SDL_PollEvent (&event))
    {
      switch (event.type)
        {
        case SDL_KEYDOWN:
          return event.key.keysym.sym;
        case SDL_QUIT:
          exit (3);
        }
    }
  return -1;
}

auto
screen::update_screen () -> void
{
  SDL_Flip (window);
}

auto
screen::is_key_pressed (int key) -> int
{
  Uint8 *mKeytable;
  int mNumkeys;
  SDL_PumpEvents ();
  mKeytable = SDL_GetKeyState (&mNumkeys);
  return mKeytable[key];
}

auto
screen::init_graph () -> int
{
  const SDL_VideoInfo *info;
  Uint8 video_bpp;
  Uint32 videoflags;

  // Initialize SDL
  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      fprintf (stderr, "Couldn't initialize SDL: %s\n", SDL_GetError ());
      return 1;
    }
  atexit (SDL_Quit);

  // Alpha blending doesn't work well at 8-bit color
  info = SDL_GetVideoInfo ();
  if (info->vfmt->BitsPerPixel > 8)
    {
      video_bpp = info->vfmt->BitsPerPixel;
    }
  else
    {
      video_bpp = 16;
    }
  videoflags = SDL_SWSURFACE | SDL_DOUBLEBUF;

  // Set 640x480 video mode
  if ((window = SDL_SetVideoMode (640, 480, video_bpp, videoflags)) == NULL)
    {
      fprintf (stderr, "Couldn't set %ix%i video mode: %s\n", 640, 480,
               SDL_GetError ());
      return 2;
    }
  return 0;
}
