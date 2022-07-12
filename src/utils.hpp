#ifndef UTILS_H
#define UTILS_H

struct coords
{
  int x;
  int y;
  coords () : x (0), y (0) {}
  coords (int p_x, int p_y) : x (p_x), y (p_y) {}
};

#endif /* UTILS_H */
