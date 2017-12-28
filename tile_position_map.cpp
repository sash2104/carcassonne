#include <cassert>
#include <iterator>
#include <set>

#include "tile.hpp"
#include "tile_position_map.hpp"

TilePositionMap::TilePositionMap(int tile_n) {
  assert(tile_n > 0);
  // ゲームに使われるタイルの枚数がtile_nのとき
  // -(tile_n - 1) <= x, y <= tile_n - 1
  // の範囲をカバーする必要がある。
  // 配列のインデックスとして負の数は使えないので内部で扱うときは
  // (x, y) -> (x + shift_, y + shift_)
  // と座標変換を行う。
  // 座標変換後のx、yの範囲は
  // 0 <= x, y <= size_ - 1
  // となる。
  shift_ = tile_n - 1;
  size_ = tile_n * 2 - 1;
  tiles_ = new Tile*[size_*size_];
  for (int i = 0, n = size_ * size_; i < n; ++i) {
    tiles_[i] = nullptr;
  }
}

TilePositionMap::~TilePositionMap() {
  delete[] tiles_;
}

void TilePositionMap::placeTile(Tile* tile, int x, int y) {
  assert(tile != nullptr);
  assert(!isTilePlaced(x, y));
  BoardPosition pos(x, y);
  int i = convertToIndex(x, y);
  tiles_[i] = tile;
  placables_.erase(pos);
  checkAndAddPlacablePosition(x, y + 1);
  checkAndAddPlacablePosition(x + 1, y);
  checkAndAddPlacablePosition(x, y - 1);
  checkAndAddPlacablePosition(x - 1, y);
}

bool TilePositionMap::isTilePlaced(int x, int y) const {
  int i = convertToIndex(x, y);
  return tiles_[i] != nullptr;
}

Tile* TilePositionMap::getPlacedTile(int x, int y) const {
  int i = convertToIndex(x, y);
  return tiles_[i];
}

const std::set<BoardPosition>* TilePositionMap::getPlacablePositions() const {
  return &placables_;
}

bool TilePositionMap::isPlacablePosition(int x, int y) const {
  BoardPosition pos(x, y);
  decltype(placables_)::iterator it = placables_.find(pos);
  return it != placables_.end();
}

Tile* TilePositionMap::undo(int x, int y) {
  assert(isTilePlaced(x, y));
  int i = convertToIndex(x, y);
  Tile* tile = tiles_[i];
  tiles_[i] = nullptr;
  checkAndAddPlacablePosition(x, y);
  checkAndRemovePlacablePosition(x, y + 1);
  checkAndRemovePlacablePosition(x + 1, y);
  checkAndRemovePlacablePosition(x, y - 1);
  checkAndRemovePlacablePosition(x - 1, y);
  return tile;
}

inline void TilePositionMap::checkAndAddPlacablePosition(int x, int y) {
  if (!isTilePlaced(x, y)) {
    BoardPosition pos(x, y);
    placables_.insert(pos);
  }
}

inline void TilePositionMap::checkAndRemovePlacablePosition(int x, int y) {
  if (!isTilePlaced(x, y + 1) && !isTilePlaced(x + 1, y) &&
      !isTilePlaced(x, y - 1) && !isTilePlaced(x - 1, y)) {
    BoardPosition pos(x, y);
    placables_.erase(pos);
  }
}

inline int TilePositionMap::convertToIndex(int x, int y) const {
  int x_shift = x + shift_;
  int y_shift = y + shift_;
  assert(x_shift >= 0 && x_shift < size_);
  assert(y_shift >= 0 && y_shift < size_);
  return x_shift + size_ * y_shift;
}
