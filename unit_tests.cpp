#include <cstdlib>
#include <set>

#include "game_context.hpp"
#include "meeple_color.hpp"
#include "region.hpp"
#include "segment.hpp"
#include "test_util.hpp"
#include "tile.hpp"
#include "tile_factory.hpp"
#include "tile_position_map.hpp"

void tile_position_map_tests() {
  TilePositionMap m(10);
  Tile tile1(0);
  Tile tile2(1);
  const std::set<BoardPosition>* s;

  test_assert("tile_position_map_tests#0", !m.isTilePlaced(0, 0));
  s = m.getPlacablePositions();
  test_assert("tile_position_map_tests#1", s->size() == 0);
  m.placeTile(&tile1, 0, 0);
  test_assert("tile_position_map_tests#2", m.isTilePlaced(0, 0));
  test_assert("tile_position_map_tests#3", m.getPlacedTile(0, 0) == &tile1);
  s = m.getPlacablePositions();
  test_assert("tile_position_map_tests#4", s->size() == 4);
  test_assert("tile_position_map_tests#5", !m.isTilePlaced(1, 0));
  m.placeTile(&tile2, 1, 0);
  test_assert("tile_position_map_tests#6", m.isTilePlaced(1, 0));
  test_assert("tile_position_map_tests#7", m.getPlacedTile(1, 0) == &tile2);
  s = m.getPlacablePositions();
  test_assert("tile_position_map_tests#8", s->size() == 6);
}

void tile_get_border_type_tests(TileFactory* tile_f) {
  Tile* tile_d = tile_f->newFromName("D", 0);
  tile_d->setRotation(0);
  test_assert("tile_get_border_type_tests#0", tile_d->getBorderType(0) == BorderType::ROAD);
  test_assert("tile_get_border_type_tests#1", tile_d->getBorderType(1) == BorderType::CITY);
  test_assert("tile_get_border_type_tests#2", tile_d->getBorderType(2) == BorderType::ROAD);
  test_assert("tile_get_border_type_tests#3", tile_d->getBorderType(3) == BorderType::FIELD);
  tile_d->setRotation(3);
  test_assert("tile_get_border_type_tests#4", tile_d->getBorderType(0) == BorderType::CITY);
  test_assert("tile_get_border_type_tests#5", tile_d->getBorderType(1) == BorderType::ROAD);
  test_assert("tile_get_border_type_tests#6", tile_d->getBorderType(2) == BorderType::FIELD);
  test_assert("tile_get_border_type_tests#7", tile_d->getBorderType(3) == BorderType::ROAD);
  delete tile_d;
}

void tile_can_adjacent_with_tests(TileFactory* tile_f) {
  Tile* tile_d = tile_f->newFromName("D", 0);
  Tile* tile_j = tile_f->newFromName("J", 1);
  tile_d->setRotation(0);
  test_assert("tile_can_adjacent_with_tests#0", tile_d->canAdjacentWith(0, tile_j, 0));
  test_assert("tile_can_adjacent_with_tests#1", !tile_d->canAdjacentWith(0, tile_j, 2));
  test_assert("tile_can_adjacent_with_tests#2", !tile_d->canAdjacentWith(3, tile_j, 0));
  test_assert("tile_can_adjacent_with_tests#3", tile_d->canAdjacentWith(3, tile_j, 2));
  tile_d->setRotation(2);
  test_assert("tile_can_adjacent_with_tests#4", tile_d->canAdjacentWith(0, tile_j, 0));
  test_assert("tile_can_adjacent_with_tests#5", !tile_d->canAdjacentWith(0, tile_j, 2));
  test_assert("tile_can_adjacent_with_tests#6", !tile_d->canAdjacentWith(3, tile_j, 0));
  test_assert("tile_can_adjacent_with_tests#7", !tile_d->canAdjacentWith(3, tile_j, 2));
  delete tile_d;
  delete tile_j;
}

void tile_is_two_segment_adjacent_tests(TileFactory* tile_f) {
  Tile* tile_j = tile_f->newFromName("J", 0);
  test_assert("tile_is_two_segment_adjacent_tests#0", tile_j->isTwoSegmentAdjacent(0, 0));
  test_assert("tile_is_two_segment_adjacent_tests#1", !tile_j->isTwoSegmentAdjacent(1, 0));
  delete tile_j;
}

void tile_tests() {
  TileFactory tile_f;
  tile_f.loadResource("tiles.json");
  tile_get_border_type_tests(&tile_f);
  tile_can_adjacent_with_tests(&tile_f);
  tile_is_two_segment_adjacent_tests(&tile_f);
}

void segment_is_adjacent_to_tests(TileFactory* tile_f) {
  Tile* tile_d = tile_f->newFromName("D", 0);
  tile_d->setRotation(0);
  Segment* s = tile_d->getCitySegments()->at(0);
  test_assert("segment_is_adjacent_to_tests#0", s->isAdjacentTo(1));
  test_assert("segment_is_adjacent_to_tests#1", !s->isAdjacentTo(3));
  tile_d->setRotation(2);
  test_assert("segment_is_adjacent_to_tests#2", !s->isAdjacentTo(1));
  test_assert("segment_is_adjacent_to_tests#3", s->isAdjacentTo(3));
  delete tile_d;
}

void segment_tests() {
  TileFactory tile_f;
  tile_f.loadResource("tiles.json");
  segment_is_adjacent_to_tests(&tile_f);
}

void cup_city_region_tests(TileFactory* tile_f) {
  Tile* tile0 = tile_f->newFromName("E", 0);
  Tile* tile1 = tile_f->newFromName("E", 1);
  tile0->setX(0).setY(0).setRotation(2);
  tile1->setX(0).setY(-1).setRotation(0);
  CityRegion region(0, nullptr);
  region.addSegment(tile0->getCitySegments()->at(0));
  test_assert("cup_city_tests#0", !region.isCompleted());
  test_assert("cup_city_tests#1", region.calculatePoint() == 1);
  region.addSegment(tile1->getCitySegments()->at(0));
  test_assert("cup_city_tests#2", region.isCompleted());
  test_assert("cup_city_tests#3", region.calculatePoint() == 4);
  delete tile0;
  delete tile1;
}

void diamond_city_region_tests(TileFactory* tile_f) {
  Tile* tile0 = tile_f->newFromName("N", 0);
  Tile* tile1 = tile_f->newFromName("N", 1);
  Tile* tile2 = tile_f->newFromName("N", 2);
  Tile* tile3 = tile_f->newFromName("P", 3);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(-1).setY(0).setRotation(1);
  tile2->setX(-1).setY(1).setRotation(2);
  tile3->setX(0).setY(1).setRotation(3);
  CityRegion region(0, nullptr);
  region.addSegment(tile0->getCitySegments()->at(0));
  region.addSegment(tile1->getCitySegments()->at(0));
  region.addSegment(tile2->getCitySegments()->at(0));
  test_assert("diamond_city_region_tests#0", !region.isCompleted());
  test_assert("diamond_city_region_tests#1", region.calculatePoint() == 3);
  region.addSegment(tile3->getCitySegments()->at(0));
  test_assert("diamond_city_region_tests#2", region.isCompleted());
  test_assert("diamond_city_region_tests#3", region.calculatePoint() == 8);
  delete tile0;
  delete tile1;
  delete tile2;
  delete tile3;
}

void diamond_city_region_with_one_pennant_tests(TileFactory* tile_f) {
  Tile* tile0 = tile_f->newFromName("N", 0);
  Tile* tile1 = tile_f->newFromName("N", 1);
  Tile* tile2 = tile_f->newFromName("M", 2);
  Tile* tile3 = tile_f->newFromName("P", 3);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(-1).setY(0).setRotation(1);
  tile2->setX(-1).setY(1).setRotation(2);
  tile3->setX(0).setY(1).setRotation(3);
  CityRegion region(0, nullptr);
  region.addSegment(tile0->getCitySegments()->at(0));
  region.addSegment(tile1->getCitySegments()->at(0));
  region.addSegment(tile2->getCitySegments()->at(0));
  test_assert("diamond_city_region_with_one_pennant_tests#0", !region.isCompleted());
  test_assert("diamond_city_region_with_one_pennant_tests#1", region.calculatePoint() == 4);
  region.addSegment(tile3->getCitySegments()->at(0));
  test_assert("diamond_city_region_with_one_pennant_tests#2", region.isCompleted());
  test_assert("diamond_city_region_with_one_pennant_tests#3", region.calculatePoint() == 10);
  delete tile0;
  delete tile1;
  delete tile2;
  delete tile3;
}

void diamond_city_region_including_i_tile_tests(TileFactory* tile_f) {
  Tile* tile0 = tile_f->newFromName("N", 0);
  Tile* tile1 = tile_f->newFromName("N", 1);
  Tile* tile2 = tile_f->newFromName("I", 2);
  Tile* tile3 = tile_f->newFromName("P", 3);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(-1).setY(0).setRotation(1);
  tile2->setX(-1).setY(1).setRotation(0);
  tile3->setX(0).setY(1).setRotation(3);
  CityRegion region(0, nullptr);
  region.addSegment(tile0->getCitySegments()->at(0));
  region.addSegment(tile1->getCitySegments()->at(0));
  region.addSegment(tile3->getCitySegments()->at(0));
  test_assert("diamond_city_region_including_i_tile_tests#0", !region.isCompleted());
  test_assert("diamond_city_region_including_i_tile_tests#1", region.calculatePoint() == 3);
  region.addSegment(tile2->getCitySegments()->at(0));
  region.addSegment(tile2->getCitySegments()->at(1));
  test_assert("diamond_city_region_including_i_tile_tests#2", region.isCompleted());
  test_assert("diamond_city_region_including_i_tile_tests#3", region.calculatePoint() == 8);
  delete tile0;
  delete tile1;
  delete tile2;
  delete tile3;
}

void city_region_tests() {
  TileFactory tile_f;
  tile_f.loadResource("tiles.json");
  cup_city_region_tests(&tile_f);
  diamond_city_region_tests(&tile_f);
  diamond_city_region_with_one_pennant_tests(&tile_f);
  diamond_city_region_including_i_tile_tests(&tile_f);
}


void cloister_region_tests() {
  Board board(10);
  CloisterRegion region(0, &board);
  Segment s(0, SegmentType::CLOISTER, false);
  Tile tile(0);
  tile.setX(0).setY(0).setRotation(0);
  s.setTile(&tile);
  region.addSegment(&s);
  TilePositionMap* map = board.getTilePositionMap();
  map->placeTile(&tile, 0, 0);
  test_assert("cloister_region_tests#0", !region.isCompleted());
  test_assert("cloister_region_tests#1", region.calculatePoint() == 1);
  for (int dx = -1; dx < 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (!(dx == 0 && dy == 0)) {
	map->placeTile(&tile, dx, dy);
      }
    }
  }
  test_assert("cloister_region_tests#2", !region.isCompleted());
  test_assert("cloister_region_tests#3", region.calculatePoint() == 6);
  for (int dy = -1; dy <= 1; dy++) {
    map->placeTile(&tile, 1, dy);
  }
  test_assert("cloister_region_tests#4", region.isCompleted());
  test_assert("cloister_region_tests#5", region.calculatePoint() == 9);
}

void loop_road_region_tests(TileFactory* tile_f) {
  Tile* tile0 = tile_f->newFromName("V", 0);
  Tile* tile1 = tile_f->newFromName("V", 1);
  Tile* tile2 = tile_f->newFromName("V", 2);
  Tile* tile3 = tile_f->newFromName("V", 3);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(0).setY(-1).setRotation(1);
  tile2->setX(-1).setY(-1).setRotation(2);
  tile3->setX(-1).setY(0).setRotation(3);
  RoadRegion region(0, nullptr);
  region.addSegment(tile0->getRoadSegments()->at(0));
  region.addSegment(tile1->getRoadSegments()->at(0));
  region.addSegment(tile2->getRoadSegments()->at(0));
  test_assert("loop_road_region_tests#0", !region.isCompleted());
  test_assert("loop_road_region_tests#1", region.calculatePoint() == 3);
  region.addSegment(tile3->getRoadSegments()->at(0));
  test_assert("loop_road_region_tests#2", region.isCompleted());
  test_assert("loop_road_region_tests#3", region.calculatePoint() == 4);
  delete tile0;
  delete tile1;
  delete tile2;
  delete tile3;
}

void loop_road_region_with_cross_road_tests(TileFactory* tile_f) {
  Tile* tile0 = tile_f->newFromName("W", 0);
  Tile* tile1 = tile_f->newFromName("V", 1);
  Tile* tile2 = tile_f->newFromName("V", 2);
  Tile* tile3 = tile_f->newFromName("V", 3);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(0).setY(-1).setRotation(1);
  tile2->setX(-1).setY(-1).setRotation(2);
  tile3->setX(-1).setY(0).setRotation(3);
  RoadRegion region(0, nullptr);
  region.addSegment(tile0->getRoadSegments()->at(1));
  region.addSegment(tile1->getRoadSegments()->at(0));
  region.addSegment(tile2->getRoadSegments()->at(0));
  test_assert("loop_road_region_with_cross_road_tests#0", !region.isCompleted());
  test_assert("loop_road_region_with_cross_road_tests#1", region.calculatePoint() == 3);
  region.addSegment(tile0->getRoadSegments()->at(2));
  region.addSegment(tile3->getRoadSegments()->at(0));
  test_assert("loop_road_region_with_cross_road_tests#2", region.isCompleted());
  test_assert("loop_road_region_with_cross_road_tests#3", region.calculatePoint() == 4);
  delete tile0;
  delete tile1;
  delete tile2;
  delete tile3;
}

void non_loop_road_region_tests(TileFactory* tile_f) {
  Tile* tile0 = tile_f->newFromName("A", 0);
  Tile* tile1 = tile_f->newFromName("U", 1);
  Tile* tile2 = tile_f->newFromName("X", 2);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(0).setY(-1).setRotation(0);
  tile2->setX(0).setY(-2).setRotation(0);
  RoadRegion region(0, nullptr);
  region.addSegment(tile0->getRoadSegments()->at(0));
  region.addSegment(tile1->getRoadSegments()->at(0));
  test_assert("non_loop_road_region_tests#0", !region.isCompleted());
  test_assert("non_loop_road_region_tests#1", region.calculatePoint() == 2);
  region.addSegment(tile2->getRoadSegments()->at(0));
  test_assert("non_loop_road_region_tests#2", region.isCompleted());
  test_assert("non_loop_road_region_tests#3", region.calculatePoint() == 3);
  delete tile0;
  delete tile1;
  delete tile2;
}

void road_region_tests() {
  TileFactory tile_f;
  tile_f.loadResource("tiles.json");
  loop_road_region_tests(&tile_f);
  loop_road_region_with_cross_road_tests(&tile_f);
  non_loop_road_region_tests(&tile_f);
}

void game_context_tests() {
  GameContext context(7);
  context.registerMeeple(MeepleColor::RED);
  context.registerMeeple(MeepleColor::GREEN);

  context.placeMeeple(MeepleColor::RED);
  context.placeMeeple(MeepleColor::RED);
  context.placeMeeple(MeepleColor::GREEN);
  context.placeMeeple(MeepleColor::GREEN);
  context.placeMeeple(MeepleColor::GREEN);
  test_assert("game_context_tests#0.0", context.getHoldingMeepleCount(MeepleColor::RED) == 5);
  test_assert("game_context_tests#0.1", context.getOnBoardMeepleCount(MeepleColor::RED) == 2);
  test_assert("game_context_tests#0.2", context.getReturnedMeepleCount(MeepleColor::RED) == 0);
  test_assert("game_context_tests#0.3", context.getHoldingMeepleCount(MeepleColor::GREEN) == 4);
  test_assert("game_context_tests#0.4", context.getOnBoardMeepleCount(MeepleColor::GREEN) == 3);
  test_assert("game_context_tests#0.5", context.getReturnedMeepleCount(MeepleColor::GREEN) == 0);

  context.addPoint(MeepleColor::RED, RegionType::CITY, 4);
  context.returnMeeple(MeepleColor::RED, 1);
  context.addPoint(MeepleColor::RED, RegionType::CLOISTER, 9);
  context.returnMeeple(MeepleColor::RED, 1);
  context.addPoint(MeepleColor::GREEN, RegionType::ROAD, 3);
  context.returnMeeple(MeepleColor::GREEN, 1);
  test_assert("game_context_tests#1.0", context.getHoldingMeepleCount(MeepleColor::RED) == 5);
  test_assert("game_context_tests#1.1", context.getOnBoardMeepleCount(MeepleColor::RED) == 0);
  test_assert("game_context_tests#1.2", context.getReturnedMeepleCount(MeepleColor::RED) == 2);
  test_assert("game_context_tests#1.3", context.getHoldingMeepleCount(MeepleColor::GREEN) == 4);
  test_assert("game_context_tests#1.4", context.getOnBoardMeepleCount(MeepleColor::GREEN) == 2);
  test_assert("game_context_tests#1.5", context.getReturnedMeepleCount(MeepleColor::GREEN) == 1);
  test_assert("game_context_tests#1.6", context.getTotalPoint(MeepleColor::RED) == 0);
  test_assert("game_context_tests#1.7", context.getGainedPoint(MeepleColor::RED) == 13);
  test_assert("game_context_tests#1.8", context.getGainedPoint(MeepleColor::RED, RegionType::CITY) == 4);
  test_assert("game_context_tests#1.9", context.getGainedPoint(MeepleColor::RED, RegionType::CLOISTER) == 9);
  test_assert("game_context_tests#1.10", context.getGainedPoint(MeepleColor::RED, RegionType::ROAD) == 0);
  test_assert("game_context_tests#1.11", context.getGainedPoint(MeepleColor::RED, RegionType::FIELD) == 0);
  test_assert("game_context_tests#1.12", context.getTotalPoint(MeepleColor::GREEN) == 0);
  test_assert("game_context_tests#1.13", context.getGainedPoint(MeepleColor::GREEN) == 3);
  test_assert("game_context_tests#1.14", context.getGainedPoint(MeepleColor::GREEN, RegionType::CITY) == 0);
  test_assert("game_context_tests#1.15", context.getGainedPoint(MeepleColor::GREEN, RegionType::CLOISTER) == 0);
  test_assert("game_context_tests#1.16", context.getGainedPoint(MeepleColor::GREEN, RegionType::ROAD) == 3);
  test_assert("game_context_tests#1.17", context.getGainedPoint(MeepleColor::GREEN, RegionType::FIELD) == 0);

  context.endTurn();
  test_assert("game_context_tests#2.0", context.getHoldingMeepleCount(MeepleColor::RED) == 7);
  test_assert("game_context_tests#2.1", context.getOnBoardMeepleCount(MeepleColor::RED) == 0);
  test_assert("game_context_tests#2.2", context.getReturnedMeepleCount(MeepleColor::RED) == 0);
  test_assert("game_context_tests#2.3", context.getHoldingMeepleCount(MeepleColor::GREEN) == 5);
  test_assert("game_context_tests#2.4", context.getOnBoardMeepleCount(MeepleColor::GREEN) == 2);
  test_assert("game_context_tests#2.5", context.getReturnedMeepleCount(MeepleColor::GREEN) == 0);
  test_assert("game_context_tests#2.6", context.getTotalPoint(MeepleColor::RED) == 13);
  test_assert("game_context_tests#2.7", context.getGainedPoint(MeepleColor::RED) == 0);
  test_assert("game_context_tests#2.8", context.getGainedPoint(MeepleColor::RED, RegionType::CITY) == 0);
  test_assert("game_context_tests#2.9", context.getGainedPoint(MeepleColor::RED, RegionType::CLOISTER) == 0);
  test_assert("game_context_tests#2.10", context.getGainedPoint(MeepleColor::RED, RegionType::ROAD) == 0);
  test_assert("game_context_tests#2.11", context.getGainedPoint(MeepleColor::RED, RegionType::FIELD) == 0);
  test_assert("game_context_tests#2.12", context.getTotalPoint(MeepleColor::GREEN) == 3);
  test_assert("game_context_tests#2.13", context.getGainedPoint(MeepleColor::GREEN) == 0);
  test_assert("game_context_tests#2.14", context.getGainedPoint(MeepleColor::GREEN, RegionType::CITY) == 0);
  test_assert("game_context_tests#2.15", context.getGainedPoint(MeepleColor::GREEN, RegionType::CLOISTER) == 0);
  test_assert("game_context_tests#2.16", context.getGainedPoint(MeepleColor::GREEN, RegionType::ROAD) == 0);
  test_assert("game_context_tests#2.17", context.getGainedPoint(MeepleColor::GREEN, RegionType::FIELD) == 0);
}

void tests() {
  tile_position_map_tests();
  tile_tests();
  segment_tests();
  cloister_region_tests();
  city_region_tests();
  road_region_tests();
  game_context_tests();
}

int main(int argc, char* argv[]) {
  start_test();
  tests();
  if (end_test() == 0) {
    std::exit(0);
  } else {
    std::exit(1);
  }
}
