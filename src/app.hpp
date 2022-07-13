#ifndef APP_H
#define APP_H

#include <memory>

// SDL forward
struct SDL_Window;

class game;
class renderer;

class app
{
public:
  app ();

  auto init (bool is_full_screen, unsigned int width, unsigned int height)
      -> bool;
  auto shut_down () -> void;
  auto run () -> void;

private:

  SDL_Window *m_window;
  renderer *m_renderer;
  game *m_game;
};

#endif /* APP_H */
