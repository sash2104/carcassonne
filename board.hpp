#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <array>
#include <string>

#include "tile_holder.hpp"

class Board {
  public:
    Board();
    int load_tile_file(std::string filename);
    static const int N_TILES = 72;
    ~Board();
  private:
    std::array<TileHolder*, N_TILES> pile_;
    int num_remain_tiles_;
};

#endif
