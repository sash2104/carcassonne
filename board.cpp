#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "board.hpp"


std::vector<std::string> split(const std::string &str, char delim){
  std::istringstream iss(str); std::string tmp; std::vector<std::string> res;
  while(getline(iss, tmp, delim)) res.push_back(tmp);
  return res;
}

Board::Board() {}

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
  int num_tiles, has_cloister, has_pennant;
  while(!ifs.eof()) {
    ifs >> tile_id >> edge_type >> num_tiles;
    ifs >> farm_connection >> city_connection >> road_connection;
    ifs >> has_cloister >> has_pennant;
    std::cout << tile_id << "," << edge_type << "," << num_tiles << "," << has_pennant << std::endl;
  }
  return 0;
}
