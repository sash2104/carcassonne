#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "board.hpp"
#include "tile.hpp"


std::vector<std::string> split(const std::string &str, char delim){
  std::istringstream iss(str); std::string tmp; std::vector<std::string> res;
  while(getline(iss, tmp, delim)) res.push_back(tmp);
  return res;
}

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

int Board::load_tile_file(std::string filename) {
  std::ifstream ifs(filename);
  if (ifs.fail())
  {
    std::cerr << filename << "を開けません" << std::endl;
    return -1;
  }
  std::string dummy;
  getline(ifs, dummy); // ignore first line
  std::string edge_type, farm_connection, city_connection, road_connection;
  int tile_id;
  int num_tiles, cloister_flag, pennant_flag;
  ifs >> tile_id >> edge_type >> num_tiles;
  ifs >> farm_connection >> city_connection >> road_connection;
  ifs >> cloister_flag >> pennant_flag;

  while(!ifs.eof()) {
    Tile tile = Tile(tile_id, num_tiles,
                     cloister_flag==1, pennant_flag==1);
    tile.setEdgeInfo(edge_type);
    tile.setRoadInfo(road_connection);
    tile.setCityInfo(city_connection);
    tile.setFarmInfo(farm_connection);
    ifs >> tile_id >> edge_type >> num_tiles;
    ifs >> farm_connection >> city_connection >> road_connection;
    ifs >> cloister_flag >> pennant_flag;

    for (int i = 0; i < tile.getNumTiles(); ++i) {
      pile_[num_remain_tiles_] = new TileHolder(tile.getId());
      ++num_remain_tiles_;
    }
  }
  return 0;
}
