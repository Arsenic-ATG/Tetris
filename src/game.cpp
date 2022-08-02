/**@file game.cpp
 * @brief containst function that implements the game logic.
 *
 */

#include "game.hpp"
#include "renderer.hpp"
#include <random>

static constexpr auto board_width = 10u;
static constexpr auto board_height = 20u;
static constexpr auto initial_frames_fall_step = 48u;

// rotation data (uses SRS)
static const tetromino tetromino_data[static_cast<int> (tetromino_type::count)]
    = {
        // I
        {
            {
                { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 } },
                { { 2, 0 }, { 2, 1 }, { 2, 2 }, { 2, 3 } },
                { { 0, 2 }, { 1, 2 }, { 2, 2 }, { 3, 2 } },
                { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 } },
            },
            0x00ffffff,
        },
        // J
        {
            {
                { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
                { { 1, 0 }, { 2, 0 }, { 1, 1 }, { 1, 2 } },
                { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 2, 2 } },
                { { 1, 0 }, { 1, 1 }, { 0, 2 }, { 1, 2 } },
            },
            0x0000ffff,
        },
        // L
        {
            {
                { { 2, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
                { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 2 } },
                { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 0, 2 } },
                { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 1, 2 } },
            },
            0xff7f00ff,
        },
        // O
        {
            {
                { { 1, 0 }, { 2, 0 }, { 1, 1 }, { 2, 1 } },
                { { 1, 0 }, { 2, 0 }, { 1, 1 }, { 2, 1 } },
                { { 1, 0 }, { 2, 0 }, { 1, 1 }, { 2, 1 } },
                { { 1, 0 }, { 2, 0 }, { 1, 1 }, { 2, 1 } },
            },
            0xffff00ff,
        },
        // S
        {
            {
                { { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } },
                { { 1, 0 }, { 1, 1 }, { 2, 1 }, { 2, 2 } },
                { { 1, 1 }, { 2, 1 }, { 0, 2 }, { 1, 2 } },
                { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } },
            },
            0x00ff00ff,
        },
        // T
        {
            {
                { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
                { { 1, 0 }, { 1, 1 }, { 2, 1 }, { 1, 2 } },
                { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 1, 2 } },
                { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } },
            },
            0x800080ff,
        },
        // Z
        {
            {
                { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
                { { 2, 0 }, { 1, 1 }, { 2, 1 }, { 1, 2 } },
                { { 0, 1 }, { 1, 1 }, { 1, 2 }, { 2, 2 } },
                { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 0, 2 } },
            },
            0xff0000ff,
        }
      };

// helpers

/**@brief check for colisions on the board
 *
 * check if the given isntanc of tetormino would collide with the board or not.
 *
 * @param Tetromino instance whose colisions need to be checked.
 * @param Instanc of board against which the collisions need to be checked.
 * @return true if the given tetromino would collide with the board, false
 * otherwise.
 */
static auto
is_overlap (const tetromino_instance &p_instance, const board &p_board)
{
  const auto tet
      = tetromino_data[static_cast<int> (p_instance.m_tetromino_type)];
  const auto block_coords = tet.block_coords[p_instance.m_rotation];

  for (auto i = 0u; i < tetromino::block_num; ++i)
    {
      const int x = p_instance.m_pos.x + block_coords[i].x;
      const int y = p_instance.m_pos.y + block_coords[i].y;

      // overlaping with p_board borders
      if (x < 0 || x >= static_cast<int> (p_board.width) || y < 0
          || y >= static_cast<int> (p_board.height))
        return true;

      if (p_board.static_blocks[x + y * p_board.width] != -1)
        return true;
    }
  return false;
}

/**@brief Set the block on the board
 *
 * Change the value of static blocks on the board.
 * @param instace of board on whose static blocks need to changed.
 * @param coordinates on board which needs to be set/unset.
 * @param value with which the staic block need to be filled.
 * @return void
 */
static auto
set_block (board &p_board, const coords i, const unsigned int val)
{
  // FIXME: assert that "i" should always be windin bounds
  p_board.static_blocks[i.x * p_board.width + i.y] = val;
}

/**@brief Default Constructor of game class
 */
game::game ()
    : m_frames_until_fall (initial_frames_fall_step),
      m_game_state (game::state::title_screen), m_score (0)
{
}

/**@brief Initialise the game
 * The function is under costruction.
 */
auto
game::init_game () const -> bool
{
  return true;
}

/**@brief get the playing field ready
 *
 * Clear and set up the playing board for playing a new game.
 */
auto
game::start_playing () -> void
{
  m_board.width = board_width;
  m_board.height = board_height;
  m_score = 0;
  m_board.static_blocks = std::vector<int> (m_board.width * m_board.height);

  for (auto i = 0u; i < m_board.height; ++i)
    {
      for (auto j = 0u; j < m_board.width; ++j)
        {
          m_board.static_blocks[i * m_board.width + j] = -1;
        }
    }
  generate_tetromino ();
  m_frames_per_fall_step = initial_frames_fall_step;
}

/**@brief Update the playing field responding to given input
 *
 * Contains tetromino movement and rotation mechanics and update the playing
 * board with their effects. The rotation of tetrominos follow Tetris's
 * standard super rotation system.
 *
 * @param input given to the game
 * @return void
 */
auto
game::update_playing (const game_input &input) -> void
{
  if (input.m_pause)
    {
      m_game_state = state::paused;
    }
  // horizontal movement
  if (input.m_move_left)
    {
      auto temp_instance = m_active_tetromino;
      --temp_instance.m_pos.x;
      if (!is_overlap (temp_instance, m_board))
        m_active_tetromino.m_pos.x = temp_instance.m_pos.x;
    }
  if (input.m_move_right)
    {
      auto temp_instance = m_active_tetromino;
      ++temp_instance.m_pos.x;
      if (!is_overlap (temp_instance, m_board))
        m_active_tetromino.m_pos.x = temp_instance.m_pos.x;
    }

  /* rotation floowing tetris SRS (// FIXME: check for special case of *I*
   * block where SRS can kick 2 blocks instea of one)*/
  if (input.m_rotate_clockwise)
    {
      auto temp_instance = m_active_tetromino;
      if (temp_instance.m_rotation == 0)
        {
          temp_instance.m_rotation = 3;
        }
      else
        {
          --temp_instance.m_rotation;
        }
      if (is_overlap (temp_instance, m_board))
        {
          temp_instance.m_pos.x = m_active_tetromino.m_pos.x - 1;
          if (!is_overlap (temp_instance, m_board))
            m_active_tetromino = temp_instance;
          else
            {
              temp_instance.m_pos.x = m_active_tetromino.m_pos.x + 1;
              if (!is_overlap (temp_instance, m_board))
                m_active_tetromino = temp_instance;
            }
        }
      else
        {
          m_active_tetromino = temp_instance;
        }
    }

  if (input.m_rotate_anticlockwise)
    {
      m_active_tetromino.m_rotation
          = (m_active_tetromino.m_rotation + 1) % tetromino::rotation_num;
    }

  // fall
  --m_frames_until_fall;
  if (m_frames_until_fall <= 0)
    {
      m_frames_until_fall = initial_frames_fall_step;

      auto temp_instance = m_active_tetromino;
      ++temp_instance.m_pos.y;
      if (is_overlap (temp_instance, m_board))
        {
          summon_tetromino_to_board (m_board, m_active_tetromino);
          if (!generate_tetromino ())
            m_game_state = state::game_over;
        }
      else
        {
          m_active_tetromino.m_pos.y = temp_instance.m_pos.y;
        }
    }

  if (input.m_soft_drop)
    {
      auto temp_instance = m_active_tetromino;
      ++temp_instance.m_pos.y;
      if (!is_overlap (temp_instance, m_board))
        {
          m_active_tetromino.m_pos.y = temp_instance.m_pos.y;
        }
    }

  if (input.m_hard_drop)
    {
      auto temp_instance = m_active_tetromino;
      while (!is_overlap (temp_instance, m_board))
        {
          ++temp_instance.m_pos.y;
        }
      --temp_instance.m_pos.y;
      summon_tetromino_to_board (m_board, temp_instance);
      if (!generate_tetromino ())
        m_game_state = state::game_over;
    }
}

/**@brief Draw the playing field
 *
 * Draw the latest state of the board and falling tetromino using the provided
 * renderer, the function only tells "What to render" on the screen, "how to
 * render that" is handeled by renderer class.
 *
 * @param renderer object which is used to render stuff
 * @return void
 */
auto
game::draw_playing (renderer &p_renderer) -> void
{
  static auto block_size_in_pixels = 32;

  // draw p_board
  auto board_width_in_pixels = m_board.width * block_size_in_pixels;
  auto board_height_in_pixels = m_board.height * block_size_in_pixels;

  auto board_offset_in_pixels = coords (0, 0);
  if (p_renderer.get_width () > board_width_in_pixels)
    {
      board_offset_in_pixels.x
          = (p_renderer.get_width () - board_width_in_pixels) / 2;
    }
  if (p_renderer.get_height () > board_height_in_pixels)
    {
      board_offset_in_pixels.y
          = (p_renderer.get_height () - board_height_in_pixels) / 2;
    }
  for (auto i = 0u; i < m_board.height; ++i)
    {
      const auto y = board_offset_in_pixels.y + i * block_size_in_pixels;
      for (auto j = 0u; j < m_board.width; ++j)
        {
          const auto x = board_offset_in_pixels.x + j * block_size_in_pixels;
          const auto block_state
              = m_board.static_blocks[i * m_board.width + j];
          auto block_rgba_color = 0x333333ff;
          if (block_state != -1)
            {
              block_rgba_color = tetromino_data[block_state].color;
            }
          p_renderer.draw_filled_rectangle (
              coords (x, y), block_size_in_pixels, block_size_in_pixels,
              block_rgba_color);
          p_renderer.draw_rectangle (coords (x, y), block_size_in_pixels,
                                     block_size_in_pixels, 0x404040ff);
        }
    }

  // active tetromino
  for (auto i = 0u; i < 4; ++i)
    {
      const auto &tet = tetromino_data[static_cast<int> (
          m_active_tetromino.m_tetromino_type)];
      const auto &block_coords
          = tet.block_coords[m_active_tetromino.m_rotation];
      auto tetromino_color_rgba = tet.color;

      const auto x = board_offset_in_pixels.x
                     + (m_active_tetromino.m_pos.x + block_coords[i].x)
                           * block_size_in_pixels;
      const auto y = board_offset_in_pixels.y
                     + (m_active_tetromino.m_pos.y + block_coords[i].y)
                           * block_size_in_pixels;

      p_renderer.draw_filled_rectangle (coords (x, y), block_size_in_pixels,
                                        block_size_in_pixels,
                                        tetromino_color_rgba);
    }

  // ghost block (represents location of current block if it were to be hard
  // dropped)
  for (auto i = 0u; i < 4; ++i)
    {
      auto ghost_block = m_active_tetromino;
      while (!is_overlap (ghost_block, m_board))
        {
          ++ghost_block.m_pos.y;
        }
      --ghost_block.m_pos.y;

      const auto &tet
          = tetromino_data[static_cast<int> (ghost_block.m_tetromino_type)];
      const auto &block_coords = tet.block_coords[ghost_block.m_rotation];
      auto tetromino_color_rgba = tet.color;

      const auto x
          = board_offset_in_pixels.x
            + (ghost_block.m_pos.x + block_coords[i].x) * block_size_in_pixels;
      const auto y
          = board_offset_in_pixels.y
            + (ghost_block.m_pos.y + block_coords[i].y) * block_size_in_pixels;

      p_renderer.draw_rectangle (coords (x, y), block_size_in_pixels,
                                 block_size_in_pixels, tetromino_color_rgba);
    }

  // print score
  p_renderer.draw_text ("Score :", { 100, 100 }, 0xffffffff);
  p_renderer.draw_text (std::to_string (m_score), { 100, 130 }, 0xffffffff);
}

// TODO: function yet to implement
auto
game::reset () -> void
{
}

// TODO: function yet to implement
auto
game::shutdown () -> void
{
}

/**@brief Update the overall state of the game.
 *
 * The function updates the info of the game object depending on the current
 * state of the game.
 *
 * @param input given to the game.
 * @param time since the last udate.
 * @return void
 */
auto
game::update (const game_input &input, float delta_time_seconds) -> void
{
  m_delta_time_seconds = delta_time_seconds;
  switch (m_game_state)
    {
    case state::title_screen:
      {
        if (input.m_start)
          {
            start_playing ();
            m_game_state = state::playing;
          }
      }
      break;
    case state::playing:
      {
        update_playing (input);
      }
      break;
    case state::paused:
      {
        if (input.m_pause)
          {
            m_game_state = state::playing;
          }
      }
      break;
    case state::game_over:
      {
        if (input.m_start)
          {
            m_game_state = state::title_screen;
          }
      }
      break;
    }
}

/**@brief draw the current screen depending on the game status
 *
 * As "title screen" and "game over" screens are simply text on screen,their
 * drawing logic is not implemented seperately as a different procedure.
 * @param renderer used to render stuff on screen.
 * @return void
 */
auto
game::draw (renderer &p_renderer) -> void
{
  switch (m_game_state)
    {
    case state::title_screen:
      {
        p_renderer.draw_text ("Press space to start",
                              coords (p_renderer.get_width () / 2 - 100,
                                      p_renderer.get_height () / 2),
                              0xffffffff);
      }
      break;
    case state::playing:
      {
        draw_playing (p_renderer);
      }
      break;
    case state::paused:
      {
        draw_playing (p_renderer);
        p_renderer.draw_text ("Paused ",
                              coords (p_renderer.get_width () / 2 - 40,
                                      p_renderer.get_height () / 2),
                              0xffffffff);
      }
      break;
    case state::game_over:
      {
        draw_playing (p_renderer);
        p_renderer.draw_text ("GAME OVER !!",
                              coords (p_renderer.get_width () / 2 - 100,
                                      p_renderer.get_height () / 2),
                              0xffffffff);
      }
      break;
    }
  // TODO: write fps info on screen
}

/**@brief Genrate a tetromino instance with random type and configuration.
 *
 * Use a pseudo random number generator to generate a tetromino with random
 * configuration and check if it is possible to bring it on board or not.
 * @return true if the tetromino can be generated. false otherwise.
 */
auto
game::generate_tetromino () -> bool
{
  std::random_device rd;
  std::mt19937 gen (rd ());

  std::uniform_int_distribution<> distrib (
      0, static_cast<int> (tetromino_type::count) - 1);

  m_active_tetromino.m_tetromino_type
      = static_cast<tetromino_type> (distrib (gen));
  m_active_tetromino.m_rotation = 0;
  m_active_tetromino.m_pos.x = (m_board.width - 4) / 2;
  m_active_tetromino.m_pos.y = 0;

  if (is_overlap (m_active_tetromino, m_board))
    return false;

  m_frames_until_fall = initial_frames_fall_step;
  return true;
}

/**@brief Bring the given tetromino instance on board.
 *
 * bring the tetromino to the board. The function also clears lines if the
 * tetromino completes a row.
 *
 * @param instance of the board on which the tetromino needss to be brought.
 * @param instance of the tetromino which needs to be brought on the boartd.
 * @return void
 */
auto
game::summon_tetromino_to_board (
    board &p_board, const tetromino_instance &p_tetromino_instance) -> void
{
  const auto &tet = tetromino_data[static_cast<int> (
      p_tetromino_instance.m_tetromino_type)];
  const auto &block_coords = tet.block_coords[p_tetromino_instance.m_rotation];
  for (auto i = 0u; i < tetromino::block_num; ++i)
    {
      const int x = p_tetromino_instance.m_pos.x + block_coords[i].x;
      const int y = p_tetromino_instance.m_pos.y + block_coords[i].y;

      // TODO: assert that tetromino doesn't go out of board

      p_board.static_blocks[x + y * p_board.width]
          = static_cast<unsigned int> (p_tetromino_instance.m_tetromino_type);
    }

  // clear rows
  for (auto y = 0u; y < p_board.height; ++y)
    {
      bool current_row_filled = true;
      for (auto x = 0u; x < p_board.width; ++x)
        {
          if (p_board.static_blocks[x + y * p_board.width] == -1)
            {
              current_row_filled = false;
              break;
            }
        }
      if (current_row_filled)
        {
          // 100 score per line clear
          m_score += 100;
          for (auto i = y; i > 0; --i)
            {
              for (auto j = 0u; j < p_board.width; ++j)
                {
                  p_board.static_blocks[j + i * p_board.width]
                      = p_board.static_blocks[j + (i - 1) * p_board.width];
                }
            }
        }
    }
}
