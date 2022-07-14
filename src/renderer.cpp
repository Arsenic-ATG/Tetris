#include "renderer.hpp"
/*TODO: location of SDL.h might change depending on the operating system.
handle that here.  */
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>

// helper functions

static auto
make_sdl_color (uint32_t rgb_color)
{
  SDL_Color color;
  color.r = (unsigned char)((rgb_color >> 24) & 0xff);
  color.g = (unsigned char)((rgb_color >> 16) & 0xff);
  color.b = (unsigned char)((rgb_color >> 8) & 0xff);
  color.a = (unsigned char)((rgb_color >> 0) & 0xff);
  return color;
}

static auto
print_renderer_info (SDL_RendererInfo &rendererInfo)
{
  printf ("Renderer: %s software=%d accelerated=%d, presentvsync=%d "
          "targettexture=%d\n",
          rendererInfo.name, (rendererInfo.flags & SDL_RENDERER_SOFTWARE) != 0,
          (rendererInfo.flags & SDL_RENDERER_ACCELERATED) != 0,
          (rendererInfo.flags & SDL_RENDERER_PRESENTVSYNC) != 0,
          (rendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) != 0);
}

// class renderer

renderer::renderer (SDL_Window &window, unsigned int width,
                    unsigned int height)
    : m_width (width), m_height (height), m_sdl_renderer (nullptr),
      m_font (nullptr)
{
  auto num_render_drivers = SDL_GetNumRenderDrivers ();
  printf ("%d render drivers:\n", num_render_drivers);
  for (auto i = 0; i < num_render_drivers; ++i)
    {
      SDL_RendererInfo renderer_info;
      SDL_GetRenderDriverInfo (i, &renderer_info);
      printf ("%d ", i);
      print_renderer_info (renderer_info);
    }
  Uint32 renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  auto renderer_index = -1;
  m_sdl_renderer
      = SDL_CreateRenderer (&window, renderer_index, renderer_flags);
  if (!m_sdl_renderer)
    {
      fprintf (stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError ());
      // TODO: terminate the program here;
    }

  SDL_RendererInfo renderer_info;
  if (SDL_GetRendererInfo (m_sdl_renderer, &renderer_info) != 0)
    {
      fprintf (stderr, "SDL_GetRendererInfo failed: %s\n", SDL_GetError ());
      // TODO: terminate the program here;
    }
  printf ("Created renderer:\n");
  print_renderer_info (renderer_info);

  int display_width, display_height;
  SDL_GetWindowSize (&window, &display_width, &display_height);
  printf ("Display size = (%d, %d)\n", display_width, display_height);
  printf ("Renderer logical size = (%u, %u)\n", width, height);
  if (display_width != static_cast<int> (width)
      || display_height != static_cast<int> (height))
    {
      printf ("logical size != display size (%u, %u) vs (%u, %u). Scaling "
              "will be applied\n",
              width, height, display_width, display_height);
    }
  const auto display_aspect
      = static_cast<double> (display_width) / display_height;
  const auto aspect = static_cast<double> (width) / height;
  if (display_aspect != display_aspect)
    {
      printf (
          "logical aspect != display aspect. Letterboxing will be applied\n");
    }

  SDL_RenderSetLogicalSize (m_sdl_renderer, width, height);

  SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY,
               "linear"); // make the scaled rendering look smoother

  int default_font_size = 32;
  m_font = TTF_OpenFont ("clacon.ttf", default_font_size);
  if (!m_font)
    {
      fprintf (stderr, "TTF_OpenFont failed: %s\n", TTF_GetError ());
      // failed to open file
      // TODO: terminate executeion and raise error here
    }
}

renderer::~renderer ()
{
  TTF_CloseFont (m_font);
  SDL_DestroyRenderer (m_sdl_renderer);
}

auto
renderer::clear () -> void
{
  SDL_SetRenderDrawColor (m_sdl_renderer, 0, 0, 0, 255);
  SDL_RenderClear (m_sdl_renderer);
}

auto
renderer::present () -> void
{
  SDL_RenderPresent (m_sdl_renderer);
}

auto
renderer::draw_rectangle (const coords loc, const int width, const int height,
                          const uint32_t rgb_color) -> void
{
  auto color = make_sdl_color (rgb_color);
  SDL_SetRenderDrawColor (m_sdl_renderer, color.r, color.g, color.b, color.a);
  SDL_Rect rect = { loc.x, loc.y, width, height };
  SDL_RenderDrawRect (m_sdl_renderer, &rect);
}

auto
renderer::draw_filled_rectangle (const coords loc, const int width,
                                 const int height, const uint32_t rgb_color)
    -> void
{
  auto color = make_sdl_color (rgb_color);
  SDL_SetRenderDrawColor (m_sdl_renderer, color.r, color.g, color.b, color.a);

  SDL_Rect rect = { loc.x, loc.y, width, height };
  SDL_RenderFillRect (m_sdl_renderer, &rect);
}

auto
renderer::draw_text (const std::string &text, const coords loc,
                     const uint32_t rgb_color) -> void
{
  draw_text (text.c_str (), loc, rgb_color);
}

auto
renderer::draw_text (const char *text, const coords loc,
                     const uint32_t rgb_color) -> void
{
  SDL_assert (text);

  SDL_Color color = make_sdl_color (rgb_color);

  SDL_Surface *sdl_surface = TTF_RenderText_Blended (m_font, text, color);
  SDL_Texture *texture
      = SDL_CreateTextureFromSurface (m_sdl_renderer, sdl_surface);
  int width, height;
  SDL_QueryTexture (texture, NULL, NULL, &width, &height);
  SDL_Rect dst_rect = { loc.x, loc.y, width, height };
  SDL_RenderCopy (m_sdl_renderer, texture, nullptr, &dst_rect);
  SDL_DestroyTexture (texture);
  SDL_FreeSurface (sdl_surface);
}
