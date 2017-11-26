#ifndef __UTILS_HPP__
#define __UTILS_HPP__

inline int modBy4(int n) {
  return n & 0x3;
}

inline int modBy8(int n) {
  return n & 0x7;
}

#endif
