#ifndef __COMMON_HPP__
#define __COMMON_HPP__

class Pos {
  public:
    Pos(int x, int y) : x_(x), y_(y) {}
    int x() const { return x_;}
    int y() const { return y_;}
  private:
    int x_, y_;
};

#endif
