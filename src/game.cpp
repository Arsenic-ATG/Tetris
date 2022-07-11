#include "game.hpp"
//#include "renderer.hpp"

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
            0xffaa00ff,
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
            0x9900ffff,
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
game::game ()
    : m_frames_until_fall (initial_frames_fall_step),
      m_game_state (game::state::title_screen)
{
}

auto
game::init_game () const -> bool
{
  return true;
}

auto
game::reset () -> void
{

}

auto
game::shutdown () -> void
{

}

auto
game::update () -> void
{

}

auto
game::draw (renderer &p_renderer) -> void
{

}

auto
game::generate_tetrimino () -> bool
{
  return true;
}

auto
game::summon_tetrimino_to_board () -> void
{

}
