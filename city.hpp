#ifndef __CITY_HPP__
#define __CITY_HPP__

#include "tile.hpp"

static const int NULL_ID = -1;

enum class ECityRelation {
  NONE, EMPTY, PARENT, CHILD
};

class City {
  public:
    City(const Tile& tile, int direction);
  private:
    int top_, right_, bottom_, left_; // 自分の各辺の都市ID(都市以外の辺は空ID)
    City *root_; // 自身とつながってる都市の中でもっとも親となる(一番先に置かれた)都市へのポインタ
    City *north_, *east_, *west_, *south_; // 自分の各隣接都市へのポインタ
    ECityRelation tr_n_, tr_e_, tr_w_, tr_s_; // 各隣接都市との関係. 先に置かれたタイルの都市が親.
};

#endif
