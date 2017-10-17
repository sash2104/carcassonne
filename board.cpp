#include <iostream>
#include <sstream>
#include <vector>

#include "board.hpp"
#include "tile.hpp"


// std::vector<std::string> split(const std::string &str, char delim){
//   std::istringstream iss(str); std::string tmp; std::vector<std::string> res;
//   while(getline(iss, tmp, delim)) res.push_back(tmp);
//   return res;
// }
//
Board::Board() :
  num_remain_tiles_(0)
{
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
