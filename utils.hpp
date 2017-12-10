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
  const static int dx[4] = {0, 1, 0, -1};
  const static int dy[4] = {1, 0, -1, 0};
  std::pair<int, int> p;
  p.first = x + dx[direction];
  p.second = y + dy[direction];
  return p;
}

#endif
