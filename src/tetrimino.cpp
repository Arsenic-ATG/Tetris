#include "tetrimino.hpp"
#include "point.hpp"
#include <array>

// TODO: fill more tetrimino data
/**
0 -> empty,
1 -> filled
2 -> pivot
*/
constexpr int tetrimino_collection[num_of_types][num_of_rotations][block_size]
                                  [block_size]
    = {
        // Square
        { { { 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0 },
            { 0, 0, 2, 1, 0 },
            { 0, 0, 1, 1, 0 },
            { 0, 0, 0, 0, 0 } },
          { { 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0 },
            { 0, 0, 2, 1, 0 },
            { 0, 0, 1, 1, 0 },
            { 0, 0, 0, 0, 0 } },
          { { 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0 },
            { 0, 0, 2, 1, 0 },
            { 0, 0, 1, 1, 0 },
            { 0, 0, 0, 0, 0 } },
          { { 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0 },
            { 0, 0, 2, 1, 0 },
            { 0, 0, 1, 1, 0 },
            { 0, 0, 0, 0, 0 } } },
      };

constexpr int initial_pos[num_of_types][num_of_rotations][2] = {
  /* Square */
  { { -2, -3 }, { -2, -3 }, { -2, -3 }, { -2, -3 } },
};

auto
tetrimino::get_mino_type (int type, int rotation, point pt) -> int
{
  return tetrimino_collection[type][rotation][pt.m_x][pt.m_y];
}

auto
tetrimino::get_initial_position (int type, int rotation) -> point
{
  return point (initial_pos[type][rotation][0],
                initial_pos[type][rotation][1]);
}
