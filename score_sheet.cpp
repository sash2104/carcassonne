#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "board.hpp"
#include "json.hpp"
#include "meeple_color.hpp"
#include "score_sheet.hpp"
#include "segment.hpp"
#include "tile.hpp"
#include "tile_factory.hpp"

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
  return SegmentType::INVALID;
}

PlacementType stringToPlacementType(std::string& str) {
  if (str == "initial") {
    return PlacementType::INITIAL;
  } else if (str == "skipped") {
    return PlacementType::SKIPPED;
  } else if (str == "regular") {
    return PlacementType::REGULAR;
  }
  return PlacementType::INVALID;
}

ScoreSheet* readScoreSheetFromFile(const std::string& file_name) {
  std::ifstream i(file_name.c_str());
  json j;
  i >> j;

  if (j.find("info") == j.end() || j["info"].find("players") == j["info"].end()) {
    return nullptr;
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
    if (placement == nullptr) {
      delete placement;
      delete score_sheet;
      return nullptr;
    }
    score_sheet->addPlacement(placement);
  }
  return score_sheet;
}

Placement* __constructPlacement(json& j, std::map<std::string, MeepleColor>& player_map) {
  std::string type_name = j["type"];
  PlacementType type = stringToPlacementType(type_name);
  if (type == PlacementType::INVALID) {
    return nullptr;
  }
  MeepleColor player;
  if (type == PlacementType::INITIAL) {
    player = MeepleColor::NOT_PLACED; // playerなしを表すため代用
  } else {
    if (j.find("player") != j.end()) {
      std::string player_name = j["player"];
      if (player_map.find(player_name) == player_map.end()) {
	return nullptr;
      }
      player = player_map[player_name];
    } else {
	return nullptr;
    }
  }
  std::string tile_name = j["tile"];
  TilePlacement* tile_placement = __constructTilePlacement(j, type);
  if (tile_placement == nullptr) {
    return nullptr;
  }
  if (j.find("meeplePlacement") == j.end()) {
    return new Placement(type, player, tile_name, tile_placement, nullptr);
  }
  MeeplePlacement* meeple_placement = __constructMeeplePlacement(j);
  if (meeple_placement == nullptr) {
    delete tile_placement;
    return nullptr;
  }
  return new Placement(type, player, tile_name, tile_placement, meeple_placement);
}

TilePlacement* __constructTilePlacement(json& j, PlacementType type) {
  if (j.find("tilePlacement") == j.end()) {
    if (type == PlacementType::INITIAL) {
      return new TilePlacement(0, 0, 0);
    } else {
      return nullptr;
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
  if (rotation < 0 || rotation >= 4) {
    return nullptr;
  }
  if (type == PlacementType::INITIAL) {
    x = y = 0;
  }
  return new TilePlacement(x, y, rotation);
}

MeeplePlacement* __constructMeeplePlacement(json& j) {
  assert(j.find("meeplePlacement") != j.end());
  std::string segment_name = j["meeplePlacement"]["segmentType"];
  SegmentType type = stringToSegmentType(segment_name);
  if (type == SegmentType::INVALID) {
    return nullptr;
  }
  int segment_index = j["meeplePlacement"]["segmentIndex"];
  MeeplePlacement* meeple_placement = new MeeplePlacement(type, segment_index);
  return meeple_placement;
}

bool validateScoreSheet(const TileFactory& tile_factory, const ScoreSheet& score_sheet) {
  const std::vector<Placement*>* placements = score_sheet.getPlacements();
  if (placements->size() == 0) {
    return false;
  }
  const std::vector<MeepleColor>* players = score_sheet.getPlayers();
  assert(players->size() == 2); // 今は二人対戦しか扱わない
  Board board(placements->size(), 7);
  board.registerMeeple(players->at(0));
  board.registerMeeple(players->at(1));

  int tile_id = 0;
  int turn = 0;
  const TilePlacement* tile_placement;
  const MeeplePlacement* meeple_placement;
  Placement* placement;
  Tile* tile;
  MeepleColor player;
  std::vector<Segment*> meeple_place_candidates;
  std::vector<std::unique_ptr<Tile>> tiles;

  placement = placements->at(turn++);
  if (placement->getType() != PlacementType::INITIAL) {
    return false;
  }
  tile = tile_factory.newFromName(placement->getTileName(), tile_id++);
  if (tile == nullptr) {
    return false;
  }
  tiles.push_back(std::unique_ptr<Tile>(tile));
  tile_placement = placement->getTilePlacement();
  board.setInitialTile(tile, tile_placement->getRotation());

  while (turn < (int) placements->size()) {
    placement = placements->at(turn++);
    tile = tile_factory.newFromName(placement->getTileName(), tile_id++);
    if (tile == nullptr) {
      return false;
    }
    tiles.push_back(std::unique_ptr<Tile>(tile));
    switch (placement->getType()) {
    case PlacementType::INITIAL:
      return false;
    case PlacementType::SKIPPED:
      if (board.hasPossiblePlacement(tile)) {
        return false;
      }
      break;
    case PlacementType::REGULAR:
      player = placement->getPlayer();
      tile_placement = placement->getTilePlacement();
      if (!board.canPlaceTile(tile, tile_placement->getX(), tile_placement->getY(), tile_placement->getRotation())) {
        return false;
      }
      board.placeTile(tile, tile_placement->getX(), tile_placement->getY(),
        tile_placement->getRotation(), &meeple_place_candidates);
      meeple_placement = placement->getMeeplePlacement();
      if (meeple_placement != nullptr) {
        bool segment_found = false;
        for (auto it = meeple_place_candidates.begin(); it != meeple_place_candidates.end(); ++it) {
          Segment* s = *it;
          if (meeple_placement->getSegmentType() == s->getType() &&
              meeple_placement->getSegmentIndex() == s->getIndex()) {
           if (!board.placeMeeple(s, player)) {
             return false;
           }
            segment_found = true;
            break;
          }
        }
        if (!segment_found) {
          return false;
        }
      }
      break;
      case PlacementType::INVALID:
	return false;
    }
    meeple_place_candidates.clear();
    board.endTurn();
  }
  board.transferRemainingPoints(true);
  board.endGame();
  const GameContext* context = board.getGameContext();
  int red_point = context->getTotalPoint(MeepleColor::RED);
  int green_point = context->getTotalPoint(MeepleColor::GREEN);
  std::cout << "RED POINT: " << red_point << ", ";
  std::cout << "GREEN POINT: " << green_point << std::endl;
  return true;
}
