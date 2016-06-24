#ifndef __TILE_HOLDER_HPP__
#define __TILE_HOLDER_HPP__

#include <array>
#include <string>

#include "tile.hpp"

static const int ROAD_MASK = 0x00f;
static const int CITY_MASK = 0x0f0;
static const int FARM_MASK = 0xf00;

static const int ROTATE_MASKS_1[4] = {0x000, 0x111, 0x333, 0x777};
static const int ROTATE_MASKS_2[4] = {0xfff, 0xeee, 0xccc, 0x888};

class TileHolder {
  public:
    TileHolder(const Tile* tile, int dir);
    TileHolder(const Tile* tile);
    void print(); // タイル表示簡易版
    int rotate(int bit_tile, int n); // タイルの回転
    int getTileId();
  private:
    const Tile* tile_;
    int dir_;
    int bit_tile_id_; // タイルの向きと辺の組み合わせ毎に固有のid
};

class LargeTileHolder {
  public:
    const int W = 7, H = 7;
    static const int BATCH_SIZE = 49;
    LargeTileHolder();
    ~LargeTileHolder();
    void setTileHolder(int x, int y, TileHolder* th);
  private:
    std::array<TileHolder*, BATCH_SIZE> batch_field_;
};

#endif
