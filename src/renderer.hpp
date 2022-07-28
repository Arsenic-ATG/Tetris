/**@file renderer.hpp
 * @brief contains function prototypes for renderer
 *
 * This contains function prototypes for the renderer driver, which is
 * responsibe for drawing stuff on screen.
 *
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "utils.hpp"
#include <SDL2/SDL_ttf.h>
#include <string>

class Texture;

struct SDL_Window;
struct SDL_Renderer;

/**@class renderer
 * @brief wrapper around SDL renderer tailerd for this application.
 *
 */
class renderer
{
public:
  renderer (SDL_Window &window, unsigned int width, unsigned int height);
  ~renderer ();

  auto clear () -> void;
  auto present () -> void;

  // getters
  auto
  get_width () const
  {
    return m_width;
  }
  auto
  get_height () const
  {
    return m_height;
  }

  auto draw_rectangle (const coords loc, const int width, const int height,
                       const uint32_t rgba_color) -> void;
  auto draw_filled_rectangle (const coords loc, const int width,
                              const int height, const uint32_t rgba_color)
      -> void;
  auto draw_text (const std::string &text, const coords loc,
                  const uint32_t rgba_color) -> void;
  auto draw_text (const char *text, const coords loc, const uint32_t rgba_color)
      -> void;

private:
  unsigned int m_width;
  unsigned int m_height;

  SDL_Renderer *m_sdl_renderer;

  TTF_Font *m_font;
};

#endif /* RENDERER_H */
