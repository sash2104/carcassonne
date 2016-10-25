#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <iostream>
#include <string>
#include <cmath>

struct Pos {
  int x, y;

  Pos(int x, int y) : x(x), y(y) {}

  friend std::ostream& operator<< (std::ostream &out, const Pos& p)
  {
    out << "(" << p.x << "," << p.y << ")";
    return out;
  }
};

#endif
