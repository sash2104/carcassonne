#include <cassert>
#include <iostream>
#include <set>

#include "board.hpp"
#include "tile_holder.hpp"

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

// 一片が都市かつ道など不正なタイルでないかチェック. (基本にないタイルでもtrue)
bool is_valid_tile(int bit_tile) {
  int road_id = bit_tile & ROAD_MASK;
  int city_id = (bit_tile & CITY_MASK) >> 4;
  int farm_id = (bit_tile & FARM_MASK) >> 8;
  // 各辺にroad, city, farmのいずれかのフラグが立っているかのチェック
  if ((road_id | city_id | farm_id) != 0x00f) { return false; }
  // road, city, farmのフラグが重複している辺がないかのチェック
  if ((road_id & city_id) || (road_id & farm_id) || (city_id & farm_id)) { return false; }
  return true;
}

int main(int argc, char const* argv[])
{
  // Board board;
  // board.load_tile_file("tiles.txt");
  std::set<int> valid_tiles;
  for (int bit_tile = 0; bit_tile <= 0xfff; ++bit_tile) {
    if (is_valid_tile(bit_tile)) {
      TileHolder tile(bit_tile, 0); tile.print();
      tile = TileHolder(bit_tile, 1); tile.print();
      tile = TileHolder(bit_tile, 2); tile.print();
      tile = TileHolder(bit_tile, 3); tile.print();
      std::cerr << std::endl;
    }
  }
  return 0;
}
