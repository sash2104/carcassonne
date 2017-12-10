#include <cassert>
#include <iostream>
#include <set>
#include <string>

#include "game.hpp"
#include "board.hpp"
#include "json.hpp"
#include "tile_factory.hpp"

using json = nlohmann::json;

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
