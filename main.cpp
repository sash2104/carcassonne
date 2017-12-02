#include <cassert>
#include <iostream>
#include <set>
#include <string>

#include "game.hpp"
#include "board.hpp"
#include "json.hpp"
#include "tile_factory.hpp"
#include "tile_holder.hpp"

using json = nlohmann::json;

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
  Game game = Game();
  while (!game.is_game_end()) {
    game.process_turn();
  }
  game.process_game_end();

  json j;
  j["pi"] = 3.141;
  std::cout << j.dump(4) << std::endl;

  std::string name("A");
  TileFactory f;
  f.loadResource("tiles.json");
  Tile* t = f.newFromName(name, 90);
  t->print_info();

  return 0;
}
