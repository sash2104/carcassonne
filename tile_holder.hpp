#ifndef __TILE_HOLDER_HPP__
#define __TILE_HOLDER_HPP__

#include <string>

static const int ROAD_MASK = 0x00f;
static const int CITY_MASK = 0x0f0;
static const int FARM_MASK = 0xf00;

static const int ROTATE_MASKS_1[4] = {0x000, 0x111, 0x333, 0x777};
static const int ROTATE_MASKS_2[4] = {0xfff, 0xeee, 0xccc, 0x888};

class TileHolder {
  public:
    TileHolder(int tile_id);
    TileHolder(int id, int dir);
    void print(); // タイル表示簡易版
    int rotate(int bit_tile, int n); // タイルの回転
    int getTileId();
  private:
    int tile_id_;
    int dir_;
    int bit_tile_id_; // タイルの向きと辺の組み合わせ毎に固有のid
};

#endif
