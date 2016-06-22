#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <array>
#include <string>

#include "tile.hpp"
#include "tile_holder.hpp"

class Board {
  public:
    Board();
    ~Board();
    static const int N_TILES = 72;

    int getCurrentTileId(int turn);
    void setPile(const std::vector<Tile*> &tiles);
  private:
    std::array<TileHolder*, N_TILES> pile_;
    int num_remain_tiles_;
};

#endif
