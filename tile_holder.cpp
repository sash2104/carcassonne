#include <cassert>
#include <iostream>
#include <string>
#include "tile_holder.hpp"

TileHolder::TileHolder(const Tile* tile, int dir) :
  tile_(tile), dir_(dir) {}

// タイルのデフォルトは無回転.
TileHolder::TileHolder(const Tile* tile) :
  tile_(tile), dir_(0) {}

int TileHolder::getTileId() { return tile_->getId(); }

const Tile* TileHolder::getTile() const { return tile_; }

// 道->r, 都市->c, 草原->fとして文字でタイルを表示.
void TileHolder::print() {
  std::string char_tile = "";
  for (int direction = 0; direction < 4; ++direction) {
    int direction_mask = 0x001 << direction;
    int road_id = bit_tile_id_;
    int city_id = bit_tile_id_ >> 4;
    int farm_id = bit_tile_id_ >> 8;
    for (int edge_type = 0; edge_type < 3; ++edge_type) {
      if ((road_id >> edge_type) & direction_mask) { char_tile += "r"; break; }
      if ((city_id >> edge_type) & direction_mask) { char_tile += "c"; break; }
      if ((farm_id >> edge_type) & direction_mask) { char_tile += "f"; break; }
      // ここに来るのはおかしい
      assert(false);
    }
  }
  std::cout << char_tile << std::endl;
}

// bit_tileをn方向だけ時計回りに回転させたbit_tileを返す
int TileHolder::rotate(int bit_tile, int n) {
  assert(n >= 0 && n <= 3);
  int rotate_bit_1, rotate_bit_2;
  // road, city, farmの各4フラグの, 上位 n bitの回転後
  rotate_bit_1 = (bit_tile & ROTATE_MASKS_1[n]) << (4-n);
  // road, city, farmの各4フラグの, 下位 n bitの回転後
  rotate_bit_2 = (bit_tile & ROTATE_MASKS_2[n]) >> n;
  return rotate_bit_1 | rotate_bit_2;
}

LargeTileHolder::LargeTileHolder() {
  for (int i = 0; i < BATCH_SIZE; ++i) { batch_field_[i] = nullptr; }
}

LargeTileHolder::~LargeTileHolder() {
  for (int i = 0; i < BATCH_SIZE; ++i) {
    if (batch_field_[i] != nullptr) { delete batch_field_[i]; }
  }
}

int LargeTileHolder::convertToLocalPosID(int x, int y) {
  // (x     , y     ) -> (lx  , ly  )
  // (W*n+dx, H*m+dy) -> ((3+dx)%7, (3+dy)%7)  に変換
  int lx, ly;
  lx = (3 + x) % W;
  ly = (3 + y) % H;
  return ly * W + lx;
}

void LargeTileHolder::setTileHolder(int pos_id, TileHolder* th) {
  batch_field_[pos_id] = th;
}

void LargeTileHolder::setTileHolder(const Pos& p, TileHolder* th) {
  batch_field_[p.y*W+p.x] = th;
}
