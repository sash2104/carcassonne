#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "json.hpp"
#include "meeple_color.hpp"
#include "score_sheet.hpp"
#include "segment.hpp"

using json = nlohmann::json;

Placement* __constructPlacement(json& j, std::map<std::string, MeepleColor>& player_map);
TilePlacement* __constructTilePlacement(json& j, PlacementType type);
MeeplePlacement* __constructMeeplePlacement(json& j);

SegmentType stringToSegmentType(std::string& str) {
  if (str == "city") {
    return SegmentType::CITY;
  } else if (str == "cloister") {
    return SegmentType::CLOISTER;
  } else if (str == "road") {
    return SegmentType::ROAD;
  } else if (str == "field") {
    return SegmentType::FIELD;
  }
  assert(false);
}

PlacementType stringToPlacementType(std::string& str) {
  if (str == "initial") {
    return PlacementType::INITIAL;
  } else if (str == "skipped") {
    return PlacementType::SKIPPED;
  } else if (str == "regular") {
    return PlacementType::REGULAR;
  }
  assert(false);
}

ScoreSheet* readScoreSheetFile(const std::string& file_name) {
  std::ifstream i(file_name.c_str());
  json j;
  i >> j;

  if (j.find("info") == j.end() || j["info"].find("players") == j["info"].end()) {
    assert(false);
  }
  json players_j = j["info"]["players"];
  std::vector<std::string> players;
  for (auto it = players_j.begin(); it != players_j.end(); it++) {
    std::string name = (*it)["name"];
    players.push_back(name);
  }
  assert(players.size() == 2); // 今は2人しか扱わない
  ScoreSheet* score_sheet = new ScoreSheet();
  std::map<std::string, MeepleColor> player_map;
  player_map[players[0]] = MeepleColor::RED;
  player_map[players[1]] = MeepleColor::GREEN;
  score_sheet->addPlayer(MeepleColor::RED);
  score_sheet->addPlayer(MeepleColor::GREEN);

  for (json::iterator it = j["placements"].begin(); it != j["placements"].end(); ++it) {
    Placement* placement = __constructPlacement(*it, player_map);
    score_sheet->addPlacement(placement);
  }
  return score_sheet;
}

Placement* __constructPlacement(json& j, std::map<std::string, MeepleColor>& player_map) {
  std::string type_name = j["type"];
  PlacementType type = stringToPlacementType(type_name);
  MeepleColor player;
  if (type == PlacementType::INITIAL) {
    player = MeepleColor::NOT_PLACED; // playerなしを表すため代用
  } else {
    if (j.find("player") != j.end()) {
      std::string player_name = j["player"];
      if (player_map.find(player_name) == player_map.end()) {
	assert(false);
      }
      player = player_map[player_name];
    } else {
      assert(false);
    }
  }
  std::string tile_name = j["tile"];
  TilePlacement* tile_placement = __constructTilePlacement(j, type);
  MeeplePlacement* meeple_placement = __constructMeeplePlacement(j);
  return new Placement(type, player, tile_name, tile_placement, meeple_placement);
}

TilePlacement* __constructTilePlacement(json& j, PlacementType type) {
  if (j.find("tilePlacement") == j.end()) {
    if (type == PlacementType::INITIAL) {
      return new TilePlacement(0, 0, 0);
    } else {
      assert(false);
    }
  }
  json tile_placement_j = j["tilePlacement"];
  int x, y, rotation;
  if (tile_placement_j.find("x") != tile_placement_j.end()) {
    x = tile_placement_j["x"];
  } else {
    x = 0;
  }
  if (tile_placement_j.find("y") != tile_placement_j.end()) {
    y = tile_placement_j["y"];
  } else {
    y = 0;
  }
  if (tile_placement_j.find("rotation") != tile_placement_j.end()) {
    rotation = tile_placement_j["rotation"];
  } else {
    rotation = 0;
  }
  assert(rotation >= 0 && rotation < 4);
  if (type == PlacementType::INITIAL) {
    x = y = 0;
  }
  return new TilePlacement(x, y, rotation);
}

MeeplePlacement* __constructMeeplePlacement(json& j) {
  if (j.find("meeplePlacement") == j.end()) {
    return nullptr;
  }
  std::string segment_name = j["meeplePlacement"]["segmentType"];
  SegmentType type = stringToSegmentType(segment_name);
  int segment_index = j["meeplePlacement"]["segmentIndex"];
  MeeplePlacement* meeple_placement = new MeeplePlacement(type, segment_index);
  return meeple_placement;
}
