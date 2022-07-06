#ifndef POINT_H
#define POINT_H

// TODO: update to supporty basic co-ordinate geometry operations;
struct point
{
  int m_x, m_y;

  point (): m_x (0), m_y(0) {}
  point (int x, int y) : m_x (x), m_y (y) {}
};


#endif /* POINT_H */
