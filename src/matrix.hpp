#ifndef MATRIX_H
#define MATRIX_H

#include "tetrimino.hpp"
#include <array>
#include <memory>

constexpr int mino_size = 16;
constexpr int matrix_width = 10;
constexpr int matrix_height = 20;

constexpr int matrix_position = 320;
constexpr int min_vertical_margin = 20;
constexpr int min_horizontal_margin = 20;

using grid = std::array<std::array<int, matrix_width>, matrix_height>;

class matrix
{
public:
  matrix (std::unique_ptr<tetrimino> tetriminos, int screen_height);

  auto get_pixel_pos (const point pos) -> point;
  auto is_empty_place (point pt) -> bool;
  auto is_movement_possible (point pt, int type, int rotation) -> bool;
  auto add_tetrimino (point pt, int type, int rotation) -> void;
  auto clear_possible_lines () -> void;
  auto is_game_over () -> bool;

private:
  std::unique_ptr<tetrimino> m_tetriminos;
  grid m_matrix;
  int m_screen_height;

  auto clear_line (int line_number) -> void;
};
#endif /* MATRIX_H */
