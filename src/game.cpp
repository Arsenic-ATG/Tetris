#include "game.hpp"
#include <memory>

game::game (std::unique_ptr<matrix> mat, std::unique_ptr<tetrimino> tet,
            std::unique_ptr<screen> src, int screen_height)
    : m_screen_height (screen_height), m_matrix (std::move (mat)),
      m_tetriminos (std::move (tet)), m_screen (std::move (src))
{
  init_game ();
}

auto
game::draw_scene () -> void
{
  draw_matrix ();

  draw_tetrimino (m_curr_pos, m_curr_type, m_curr_rotation);
  draw_tetrimino (m_next_pos, m_next_type, m_next_rotation);
}

auto
game::create_new_tetrimino () -> void
{
  m_curr_type = m_next_type;
  m_curr_rotation = m_next_rotation;
  auto initial_pos
      = m_tetriminos->get_initial_position (m_curr_type, m_curr_rotation);
  m_curr_pos = point (initial_pos.m_x, (matrix_width / 2) + initial_pos.m_y);

  // TODO: randomly generate new tetirmnos
  m_next_type = 0;
  m_next_rotation = 0;
}

auto
game::init_game () -> void
{
}

auto
game::draw_tetrimino (const point pos, const int type, const int rotation)
    -> void
{
  color mino_color;

  auto pixel_pos = point (m_matrix->get_pixel_pos (pos));

  for (auto i = 0; i < block_size; ++i)
    {
      for (auto j = 0; j < block_size; ++j)
        {
          switch (m_tetriminos->get_mino_type (type, rotation, point (i, j)))
            {
            case 1:
              mino_color = color::green;
              break;
            case 2:
              mino_color = color::blue;
              break;
            }

          if (m_tetriminos->get_mino_type (type, rotation, point (i, j))x)
            {
              auto p1 = point (pixel_pos.m_x + i * mino_size,
                               pixel_pos.m_y + j * mino_size);
              auto p2 = point (p1.m_x + mino_size - 1, p1.m_y + mino_size - 1);
              m_screen->draw_rectangle (p1, p2, mino_color);
            }
        }
    }
}
