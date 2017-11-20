#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

#include "board.hpp"
#include "region.hpp"
#include "segment.hpp"
#include "tile.hpp"
#include "tile_holder.hpp"


// std::vector<std::string> split(const std::string &str, char delim){
//   std::istringstream iss(str); std::string tmp; std::vector<std::string> res;
//   while(getline(iss, tmp, delim)) res.push_back(tmp);
//   return res;
// }
//
Board::Board(int tile_n) : tile_map_(tile_n) {
  for (int i = 0; i < N_TILES; ++i) { pile_[i] = nullptr; }
  for (int i = 0; i < FIELD_SIZE; ++i) { field_[i] = nullptr; }
}

Board::~Board() {
  for (LargeTileHolder* lth : field_) { if (lth != nullptr) { delete lth; } };
}

const Tile* Board::getCurrentTile(int turn) const { return pile_[turn]; }

void Board::setPile(const std::vector<Tile*> & tiles) {
  for (const Tile* tile : tiles) {
    for (int i = 0; i < tile->getNumTiles(); ++i) {
      pile_[num_remain_tiles_] = tile;
      ++num_remain_tiles_;
    }
  }
}

int Board::getFieldPosId(int x, int y) {
  // TODO: Field中のLargeTileHolderの位置に変える. (10, 10)が初期位置
  int fx, fy;
  fx = (x+3) / LargeTileHolder::W + 10;
  fy = (y+3) / LargeTileHolder::H + 10;
  return fy * 21 + fx;
}

// (0, 0)を初期位置とした時の(x, y)にタイルをdirの向きに配置
void Board::placeTile(const Tile* tile, int dir, int x, int y) {
  int pos_id = getFieldPosId(x, y);
  if (field_[pos_id] == nullptr) { field_[pos_id] = new LargeTileHolder(); }
  LargeTileHolder* lth = field_[pos_id];
  int lth_pos_id = lth->convertToLocalPosID(x, y);
  lth->setTileHolder(lth_pos_id, new TileHolder(tile, dir));
}

void Board::printField() {
  std::cout << "Field : ";
  for (int pos_id = 0; pos_id < FIELD_SIZE; ++pos_id) {
    if (field_[pos_id] == nullptr) { continue; }
    std::cout << "(" << pos_id % 21 << "," << pos_id / 21 << ")";
  }
  std::cout << std::endl;
}

const TilePositionMap* Board::getTilePositionMap() const {
  return &tile_map_;
}

const std::vector<CityRegion*>* Board::getCityRegions() const {
  return &city_regions_;
}

const std::vector<CloisterRegion*>* Board::getCloisterRegions() const {
  return &cloister_regions_;
}

const std::vector<FieldRegion*>* Board::getFieldRegions() const {
  return &field_regions_;
}

const std::vector<RoadRegion*>* Board::getRoadRegions() const {
  return &road_regions_;
}

bool Board::canPlaceTile(Tile* tile, int x, int y, int rotation) {
  if (!tile_map_.isPlacablePosition(x, y)) {
    return false;
  }
  return isAdjacencyValid(tile, x, y, rotation);
}

bool Board::hasPossiblePlacement(Tile* tile) {
  // TODO
  return false;
}

bool Board::isAdjacencyValid(Tile* tile, int x, int y, int rotation) {
  // TODO
  return false;
}

void Board::setInitialTile(Tile* tile) {
  setInitialTile(tile, 0);
}

void Board::setInitialTile(Tile* tile, int rotation) {
  assert(rotation >= 0 && rotation < 4);
  // TODO
}

bool Board::placeTile(Tile* tile, int x, int y, int rotation, std::vector<Segment*>* meeplePlaceCandidates) {
  if (!canPlaceTile(tile, x, y, rotation)) {
    return false;
  }
  // TODO
  return true;
}

bool Board::placeMeeple(Segment* segment, int meepleColor) {
  Region* region = segment->getRegion();
  if (region->meepleIsPlaced()) {
    return false;
  }
  segment->placeMeeple(meepleColor);
  if (region->isCompleted()) {
    // TODO: 色々処理
  }
  return true;
}
