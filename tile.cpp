#include <cassert>
#include <iostream>

#include "tile.hpp"

// 2進数で表示
void print_bit_tile(int bit_tile) {
  int bit_offset = 2048;
  while (bit_offset > 0) {
    if (bit_tile & bit_offset) { std::cerr << 1; }
    else { std::cerr << 0; }
    bit_offset /= 2;
  }
  std::cerr << std::endl;
}

Tile::Tile(int id, int num_tiles, bool has_cloister, bool has_pennant)
  : id_(id), num_tiles_(num_tiles),
    has_cloister_(has_cloister), has_pennant_(has_pennant) {
    }

int Tile::getId() const { return id_; }

int Tile::getNumTiles() const { return num_tiles_; }

void Tile::setEdgeInfo(std::string edge_type) {
  assert(edge_type.size() == 4);
  int edge_id = 0x000;
  for (int i = 0; i < 4; ++i) {
    int edge_offset = i;
    switch(edge_type[i]) {
      case 'r': // road
        break;
      case 'c': // city
        edge_offset += 4;
        break;
      case 'f': // farm
        edge_offset += 8;
        break;
      default:
        std::cerr << "Error: invalid edge type." << std::endl;
        assert(false);
    }
    edge_id |= (1 << edge_offset);
  }
  // std::cout << id_ << " " << edge_type << std::endl;
  edge_id_ = edge_id;
  // print_bit_tile(edge_id_);
}

// つながっている道は同じid, 分かれている道は異なるidとなるように各道にidを付与
void Tile::setRoadInfo(std::string road_connection) {
  assert(road_connection.size() == 4);
  for (int i = 0; i < 4; ++i) {
    roads_[i] = road_connection[i] - '0';
    // std::cout << roads_[i];
  }
  // std::cout << std::endl;
}

// つながっている都市は同じid, 分かれている都市は異なるidとなるように各都市にidを付与
void Tile::setCityInfo(std::string city_connection) {
  assert(city_connection.size() == 4);
  for (int i = 0; i < 4; ++i) {
    cities_[i] = city_connection[i] - '0';
    // std::cout << cities_[i];
  }
  // std::cout << std::endl;
}

// つながっている草原は同じid, 分かれている草原は異なるidとなるように各草原にidを付与
void Tile::setFarmInfo(std::string farm_connection) {
  assert(farm_connection.size() == 9);
  for (int i = 0; i < 9; ++i) {
    farms_[i] = farm_connection[i] - '0';
    // std::cout << farms_[i];
  }
  // std::cout << std::endl;
}
