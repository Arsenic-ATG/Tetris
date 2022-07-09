#include "game.hpp"
//#include "renderer.hpp"

static constexpr auto board_width = 10u;
static constexpr auto board_height = 20u;
// static constexpr auto s_initialFramesPerFallStep = 48u;
// static constexpr auto s_deltaFramesPerFallStepPerLevel = 2;

//static const tetrimino s_tetrominos[static_cast<int> (tetrimino::type::count)]; // init later

game::game () : m_game_state (game::state::title_screen) {}

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
