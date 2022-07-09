#ifndef GAME_H
#define GAME_H

#include "matrix.hpp"
#include "point.hpp"
#include "screen.hpp"
#include "tetrimino.hpp"

class game
{
public:
  game (std::unique_ptr<matrix> mat, std::unique_ptr<tetrimino> tet,
        std::unique_ptr<screen> src, int screen_height);

  auto draw_scene () -> void;
  auto create_new_tetrimino () -> void;

  // getters
  auto
  get_curr_type () const
  {
    return m_curr_type;
  }
  auto
  get_curr_pos () const
  {
    return m_curr_pos;
  }
  auto
  get_curr_rotation () const
  {
    return m_curr_rotation;
  }

private:
  int m_screen_height;

  int m_next_type;
  point m_next_pos;
  int m_next_rotation;

  int m_curr_type;
  point m_curr_pos;
  int m_curr_rotation;

  std::unique_ptr<matrix> m_matrix;
  std::unique_ptr<tetrimino> m_tetriminos;
  std::unique_ptr<screen> m_screen;

  auto init_game () -> void;
  auto draw_tetrimino (const point p, const int type, const int rotation)
      -> void;
  auto draw_matrix () -> void;
};

#endif /* GAME_H */

void
Game::DrawPiece (int pX, int pY, int pPiece, int pRotation)
{
  color mColor; // Color of the block

  // Obtain the position in pixel in the screen of the block we want to draw
  int mPixelsX = mBoard->GetXPosInPixels (pX);
  int mPixelsY = mBoard->GetYPosInPixels (pY);

  // Travel the matrix of blocks of the piece and draw the blocks that are
  // filled
  for (int i = 0; i < PIECE_BLOCKS; i++)
    {
      for (int j = 0; j < PIECE_BLOCKS; j++)
        {
          // Get the type of the block and draw it with the correct color
          switch (mPieces->GetBlockType (pPiece, pRotation, j, i))
            {
            case 1:
              mColor = GREEN;
              break; // For each block of the piece except the pivot
            case 2:
              mColor = BLUE;
              break; // For the pivot
            }

          if (mPieces->GetBlockType (pPiece, pRotation, j, i) != 0)
            mIO->DrawRectangle (
                mPixelsX + i * BLOCK_SIZE, mPixelsY + j * BLOCK_SIZE,
                (mPixelsX + i * BLOCK_SIZE) + BLOCK_SIZE - 1,
                (mPixelsY + j * BLOCK_SIZE) + BLOCK_SIZE - 1, mColor);
        }
    }
}
