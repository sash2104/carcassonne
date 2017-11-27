#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <cassert>
#include <utility>

inline int modBy4(int n) {
  return n & 0x3;
}

inline int modBy8(int n) {
  return n & 0x7;
}

inline std::pair<int, int> adjacentCoordinate(int x, int y, int direction) {
  assert(direction >= 0 && direction < 4);
  std::pair<int, int> p;
  switch (direction) {
  case 0:
    p.first = x;
    p.second = y + 1;
    return p;
  case 1:
    p.first = x + 1;
    p.second = y;
    return p;
  case 2:
    p.first = x;
    p.second = y - 1;
    return p;
  case 3:
    p.first = x - 1;
    p.second = y;
    return p;
  default:
    // ここには到達しない
    assert(0);
  }
}

#endif
