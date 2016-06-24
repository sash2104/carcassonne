#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <array>
#include <string>
#include <unordered_map>

#include "tile.hpp"
#include "tile_holder.hpp"

class Board {
  public:
    Board();
    ~Board();
    static const int N_TILES = 72;

    int getCurrentTileId(int turn);
    int getPosId(int x, int y);
    void setPile(const std::vector<Tile*> &tiles);
    void placeTile(const Tile* tile, int dir, int x, int y); // field_の(x, y)にタイルをdirの向きに配置
  private:
    std::array<TileHolder*, N_TILES> pile_; // 未配置のタイルを管理
    std::unordered_map<int, TileHolder*> field_; // 配置済みのタイルを管理
    int num_remain_tiles_;
};

#endif
