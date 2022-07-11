#include "renderer.hpp"
#include "SDL.h"
#include <cstdio>

// helper functions

static auto
make_sdl_color (uint32_t rgb_color)
{
  SDL_Color color;
  color.r = (unsigned char)((rgba >> 24) & 0xff);
  color.g = (unsigned char)((rgba >> 16) & 0xff);
  color.b = (unsigned char)((rgba >> 8) & 0xff);
  color.a = (unsigned char)((rgba >> 0) & 0xff);
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
}

renderer::~renderer () {}

auto
renderer::clear () -> void
{
}

auto
renderer::present () -> void
{
}

auto
renderer::draw_rectangle (const coords loc, const int height,
                          const uint32_t rgb_color) -> void
{
}

auto
renderer::draw_filled_rectangle (const coords loc, const int height,
                                 const uint32_t rgb_color) -> void
{
}

auto
renderer::draw_text (const std::string &text, const coords loc,
                     const uint32_t rgb_color) -> void
{
}
