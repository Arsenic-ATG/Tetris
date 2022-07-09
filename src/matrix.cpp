#include "matrix.hpp"

matrix::matrix (std::unique_ptr<tetrimino> tetriminos, int screen_height)
    : m_tetriminos (std::move (tetriminos)), m_screen_height (screen_height)
{
  m_matrix.fill ({ 0 });
}

auto
matrix::get_pixel_pos (const point pos) -> point
{
  auto x_pos = ((m_screen_height - (mino_size * matrix_height))
                + (pos.m_x * block_size));

  auto y_pos = ((matrix_position - (mino_size * (matrix_width / 2)))
                + (pos.m_y * block_size));

  return point (x_pos, y_pos);
}

auto
matrix::is_empty_place (point pt) -> bool
{
  return (m_matrix[pt.m_x][pt.m_y]);
}

auto
matrix::is_movement_possible (point pt, int type, int rotation) -> bool
{
  for (auto i = pt.m_x; i < pt.m_x + block_size; ++i)
    {
      for (auto j = pt.m_y; j < pt.m_y + block_size; ++j)
        {
          // tetrinminmo out of bounds
          if (j <= 0 || i >= matrix_height || j >= matrix_width)
            if (m_tetriminos->get_mino_type (type, rotation,
                                             point (i - pt.m_x, j - pt.m_y)))
              return false;

          if (i >= 0)
            {
              if (m_tetriminos->get_mino_type (type, rotation,
                                               point (i - pt.m_x, j - pt.m_y))
                  && (!is_empty_place (point (i, j))))
                return false;
            }
        }
    }
  return true;
}

auto
matrix::add_tetrimino (point pt, int type, int rotation) -> void
{
  for (auto i = pt.m_x; i < pt.m_x + block_size; ++i)
    {
      for (auto j = pt.m_y; j < pt.m_y + block_size; ++j)
        {
          if (m_tetriminos->get_mino_type (type, rotation,
                                           point (i - pt.m_x, j - pt.m_y)))
            m_matrix[i][j] = 1;
        }
    }
}

auto
matrix::clear_possible_lines () -> void
{
  for (auto i = 0; i < matrix_height; ++i)
    {
      auto j = 0;
      for (; j < matrix_width; ++j)
        {
          if (!m_matrix[i][j])
            break;
        }
      if (j == matrix_width)
        clear_line (i);
    }
}

auto
matrix::is_game_over () -> bool
{
  for (auto i = 0; i < matrix_width; ++i)
    if (m_matrix[0][i])
      return true;
  return false;
}

auto
matrix::clear_line (int line_number) -> void
{
  /*
    shift all the rose one block above
  */
  for (auto i = line_number; i > 0; --i)
    {
      for (auto j = 0; j < matrix_width; ++j)
        {
          m_matrix[i][j] = m_matrix[i - i][j];
        }
    }
}
