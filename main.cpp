#include <cassert>
#include <iostream>
#include <memory>
#include <set>
#include <string>

#include "game.hpp"
#include "board.hpp"
#include "json.hpp"
#include "score_sheet.hpp"
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

  TileFactory f;
  f.loadResource("tiles.json");
  std::string file_name("doc/championship_japan_2015_final.json");
  std::unique_ptr<ScoreSheet> score_sheet(readScoreSheetFromFile(file_name));
  if (score_sheet.get() == nullptr) {
    std::cout << "Failed to read score sheet file" << std::endl;
  } else {
    bool is_valid = validateScoreSheet(f, *score_sheet.get());
    if (is_valid) {
      std::cout << "valid" << std::endl;
    } else {
      std::cout << "invaid" << std::endl;
    }
  }

  return 0;
}
