#ifndef TETRIMINO_H
#define TETRIMINO_H

#include "point.hpp"

constexpr int num_of_types = 7;
constexpr int num_of_rotations = 4;
constexpr int block_size = 5;

class tetrimino
{
  public:
  auto get_mino_type (int type, int rotation, point pt) -> int;
  auto get_intial_position (int type, int rotation) -> point;
};

#endif /* TETRIMINO_H */
