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

}

Board::~Board() {
  for (TileHolder* th : pile_) {
    if (th != nullptr) { delete th; }
  };
}

int Board::getCurrentTileId(int turn) { return pile_[turn]->getTileId(); }

void Board::setPile(const std::vector<Tile*> & tiles) {
  for (const Tile* tile : tiles) {
    for (int i = 0; i < tile->getNumTiles(); ++i) {
      pile_[num_remain_tiles_] = new TileHolder(tile->getId());
      ++num_remain_tiles_;
    }
  }
}
