/**@file game.hpp
 * @brief contains function prototypes for game
 *
 * This contains function prototypes responsible for the game logic.
 */

#ifndef GAME_H
#define GAME_H

#include "utils.hpp"
#include <memory>
#include <vector>

class renderer;

struct tetromino
{
  static constexpr auto block_num = 4u;
  static constexpr auto rotation_num = 4u;

  coords block_coords[block_num][rotation_num];

  unsigned int color;
};

enum class tetromino_type
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

struct tetromino_instance
{
  tetromino_type m_tetromino_type;
  tetromino_type next_m_tetromino_type;
  coords m_pos;
  unsigned int m_rotation;
};

struct board
{
  unsigned int width;
  unsigned int height;
  std::vector<int> static_blocks;
};

struct game_input
{
  bool m_start;
  bool m_pause;
  bool m_move_left;
  bool m_move_right;
  bool m_rotate_clockwise;
  bool m_rotate_anticlockwise;
  bool m_hard_drop;
  bool m_soft_drop;
  bool m_reset;
};

class game
{
public:
  game ();

  auto init_game () const -> bool;

  auto start_playing () -> void;
  auto update_playing (const game_input &input) -> void;
  auto draw_smalltetromino (renderer &p_renderer, int which_m_type, int x0, int y0 ) -> void;
  auto draw_playing (renderer &p_renderer) -> void;

  auto reset () -> void;
  auto shutdown () -> void;
  auto update (const game_input &input, float delta_time_seconds) -> void;
  auto draw (renderer &p_renderer) -> void;

private:
  auto generate_tetromino () -> bool;
  auto summon_tetromino_to_board (
      board &p_board, const tetromino_instance &p_tetromino_instance) -> void;

  enum class state
  {
    title_screen,
    playing,
    paused,
    game_over,
  };

  std::vector<int> bag; // bag of tetrominos (fill it with 7 pieces until empty, then fill again - 7-bag randomizer) 
  int m_frames_until_fall;
  int m_frames_per_fall_step; // control speed of the game
  tetromino_instance m_active_tetromino;
  board m_board;
  state m_game_state;
  float m_delta_time_seconds;

  // used for calculating score
  long int m_score;
  int m_lines_cleared;

};

#endif /* GAME_H */
