#ifndef GAME_H
#define GAME_H

#include "utils.hpp"

class renderer;

struct tetrimino
{
  static constexpr auto block_num = 4u;
  static constexpr auto rotation_num = 4u;

  coords block_coords[block_num][rotation_num];

  unsigned int color;
  enum class type
  {
    I,
    J,
    L,
    O,
    S,
    T,
    Z,
    count
  };
};


struct GameInput
{
  bool m_start;
  bool m_pause;
  bool m_move_left;
  bool m_move_right;
  bool m_rotate_clockwise;
  bool m_rotate_anticlockwise;
  bool m_hard_drop;
  bool m_soft_drop;
};

class game
{
public:
  game ();

  auto init_game () const -> bool;
  auto reset () -> void;
  auto shutdown () -> void;
  auto update () -> void;
  auto draw (renderer &p_renderer) -> void;

private:
  auto generate_tetrimino () -> bool;
  auto summon_tetrimino_to_board () -> void;

  enum class state
  {
    title_screen,
    playing,
    game_over,
  };

  state m_game_state;
};

#endif /* GAME_H */
