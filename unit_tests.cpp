#include <cstdlib>
#include <set>

#include "board.hpp"
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
  Tile* tp;
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
  tp = m.undo(1, 0);
  test_assert("tile_position_map_tests#9", tp == &tile2);
  test_assert("tile_position_map_tests#10", !m.isTilePlaced(1, 0));
  s = m.getPlacablePositions();
  test_assert("tile_position_map_tests#11", s->size() == 4);
}

void tile_get_border_type_tests(TileFactory* tile_factory) {
  Tile* tile_d = tile_factory->newFromName("D", 0);
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

void tile_can_adjacent_with_tests(TileFactory* tile_factory) {
  Tile* tile_d = tile_factory->newFromName("D", 0);
  Tile* tile_j = tile_factory->newFromName("J", 1);
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

void tile_is_two_segment_adjacent_tests(TileFactory* tile_factory) {
  Tile* tile_j = tile_factory->newFromName("J", 0);
  test_assert("tile_is_two_segment_adjacent_tests#0", tile_j->isTwoSegmentAdjacent(0, 0));
  test_assert("tile_is_two_segment_adjacent_tests#1", !tile_j->isTwoSegmentAdjacent(1, 0));
  delete tile_j;
}

void tile_tests() {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  tile_get_border_type_tests(&tile_factory);
  tile_can_adjacent_with_tests(&tile_factory);
  tile_is_two_segment_adjacent_tests(&tile_factory);
}

void segment_is_adjacent_to_tests(TileFactory* tile_factory) {
  Tile* tile_d = tile_factory->newFromName("D", 0);
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
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  segment_is_adjacent_to_tests(&tile_factory);
}

int count_region_segments(Region* region) {
  int count = 0;
  for (auto it = region->segmentBegin(); it != region->segmentEnd(); ++it) {
    count++;
  }
  return count;
}

void region_iterator_tests() {
  Segment s0(0, SegmentType::CITY, false);
  Segment s1(1, SegmentType::CITY, false);
  Segment s2(2, SegmentType::CITY, false);
  Segment s3(3, SegmentType::CITY, false);
  Segment s4(4, SegmentType::CITY, false);
  Segment s5(5, SegmentType::CITY, false);
  Segment s6(6, SegmentType::CITY, false);
  Segment s7(7, SegmentType::CITY, false);
  Segment s8(8, SegmentType::CITY, false);
  CityRegion r0(0, &s0, nullptr);
  CityRegion r1(1, &s3, nullptr);
  CityRegion r2(2, &s4, nullptr);
  CityRegion r3(3, &s6, nullptr);

  r0.addSegment(&s1);
  r0.addSegment(&s2);
  r2.addSegment(&s5);
  r3.addSegment(&s7);
  r3.addSegment(&s8);
  test_assert("region_iterator_tests#0", count_region_segments(&r0) == 3);
  test_assert("region_iterator_tests#1", count_region_segments(&r1) == 1);
  test_assert("region_iterator_tests#2", count_region_segments(&r2) == 2);
  test_assert("region_iterator_tests#3", count_region_segments(&r3) == 3);
  r2.mergeRegion(&r3);
  test_assert("region_iterator_tests#4", count_region_segments(&r2) == 5);
  r0.mergeRegion(&r1);
  test_assert("region_iterator_tests#5", count_region_segments(&r0) == 4);
  r0.mergeRegion(&r2);
  test_assert("region_iterator_tests#6", count_region_segments(&r0) == 9);
  test_assert("region_iterator_tests#7", count_region_segments(&r1) == 1);
  test_assert("region_iterator_tests#8", count_region_segments(&r2) == 5);
  test_assert("region_iterator_tests#9", count_region_segments(&r3) == 3);
  r0.undoMergeRegion(&r2);
  test_assert("region_iterator_tests#10", count_region_segments(&r0) == 4);
  r0.undoMergeRegion(&r1);
  test_assert("region_iterator_tests#11", count_region_segments(&r0) == 3);
}

void cup_city_region_tests(TileFactory* tile_factory) {
  Tile* tile0 = tile_factory->newFromName("E", 0);
  Tile* tile1 = tile_factory->newFromName("E", 1);
  tile0->setX(0).setY(0).setRotation(2);
  tile1->setX(0).setY(-1).setRotation(0);
  CityRegion region(0, tile0->getCitySegments()->at(0), nullptr);
  test_assert("cup_city_tests#0", !region.isCompleted());
  test_assert("cup_city_tests#1", region.calculatePoint() == 1);
  region.addSegment(tile1->getCitySegments()->at(0));
  test_assert("cup_city_tests#2", region.isCompleted());
  test_assert("cup_city_tests#3", region.calculatePoint() == 4);
  delete tile0;
  delete tile1;
}

void diamond_city_region_tests(TileFactory* tile_factory) {
  Tile* tile0 = tile_factory->newFromName("N", 0);
  Tile* tile1 = tile_factory->newFromName("N", 1);
  Tile* tile2 = tile_factory->newFromName("N", 2);
  Tile* tile3 = tile_factory->newFromName("P", 3);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(-1).setY(0).setRotation(1);
  tile2->setX(-1).setY(1).setRotation(2);
  tile3->setX(0).setY(1).setRotation(3);
  CityRegion region(0, tile0->getCitySegments()->at(0), nullptr);
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

void diamond_city_region_with_one_pennant_tests(TileFactory* tile_factory) {
  Tile* tile0 = tile_factory->newFromName("N", 0);
  Tile* tile1 = tile_factory->newFromName("N", 1);
  Tile* tile2 = tile_factory->newFromName("M", 2);
  Tile* tile3 = tile_factory->newFromName("P", 3);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(-1).setY(0).setRotation(1);
  tile2->setX(-1).setY(1).setRotation(2);
  tile3->setX(0).setY(1).setRotation(3);
  CityRegion region(0, tile0->getCitySegments()->at(0), nullptr);
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

void diamond_city_region_including_i_tile_tests(TileFactory* tile_factory) {
  Tile* tile0 = tile_factory->newFromName("N", 0);
  Tile* tile1 = tile_factory->newFromName("N", 1);
  Tile* tile2 = tile_factory->newFromName("I", 2);
  Tile* tile3 = tile_factory->newFromName("P", 3);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(-1).setY(0).setRotation(1);
  tile2->setX(-1).setY(1).setRotation(0);
  tile3->setX(0).setY(1).setRotation(3);
  CityRegion region(0, tile0->getCitySegments()->at(0), nullptr);
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
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  cup_city_region_tests(&tile_factory);
  diamond_city_region_tests(&tile_factory);
  diamond_city_region_with_one_pennant_tests(&tile_factory);
  diamond_city_region_including_i_tile_tests(&tile_factory);
}


void cloister_region_tests() {
  Board board(10, 7);
  Segment s(0, SegmentType::CLOISTER, false);
  Tile tile(0);
  tile.setX(0).setY(0).setRotation(0);
  s.setTile(&tile);
  CloisterRegion region(0, &s, &board);
  TilePositionMap* map = const_cast<TilePositionMap*>(board.getTilePositionMap());
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

void field_region_tests() {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");

  Tile* tile_e0 = tile_factory.newFromName("E", 0);
  Tile* tile_e1 = tile_factory.newFromName("E", 1);
  Tile* tile_e2 = tile_factory.newFromName("E", 2);
  Tile* tile_e3 = tile_factory.newFromName("E", 3);
  Tile* tile_e4 = tile_factory.newFromName("E", 4);
  Tile* tile_e5 = tile_factory.newFromName("E", 5);
  Tile* tile_d  = tile_factory.newFromName("D", 6);
  tile_e0->setX(0).setY(0).setRotation(0);
  tile_e1->setX(0).setY(1).setRotation(2);
  tile_e2->setX(-1).setY(0).setRotation(3);
  tile_e3->setX(-2).setY(0).setRotation(1);
  tile_e4->setX(0).setY(-1).setRotation(2);
  tile_e5->setX(2).setY(0).setRotation(3);
  tile_d->setX(1).setY(0).setRotation(0);

  FieldRegion field_region(0, tile_e0->getFieldSegments()->at(0), nullptr);
  field_region.addSegment(tile_e2->getFieldSegments()->at(0));
  field_region.addSegment(tile_e4->getFieldSegments()->at(0));
  field_region.addSegment(tile_d->getFieldSegments()->at(1));
  CityRegion city_region0(1, tile_e0->getCitySegments()->at(0), nullptr);
  city_region0.addSegment(tile_e1->getCitySegments()->at(0));
  CityRegion city_region1(2, tile_e2->getCitySegments()->at(0), nullptr);
  city_region1.addSegment(tile_e3->getCitySegments()->at(0));
  CityRegion city_region2(3, tile_d->getCitySegments()->at(0), nullptr);
  city_region2.addSegment(tile_e5->getCitySegments()->at(0));
  CityRegion city_region3(2, tile_e4->getCitySegments()->at(0), nullptr);
  test_assert("field_region_tests#0", city_region0.isCompleted());
  test_assert("field_region_tests#1", city_region1.isCompleted());
  test_assert("field_region_tests#2", city_region2.isCompleted());
  test_assert("field_region_tests#3", !city_region3.isCompleted());

  std::vector<CityRegion*> city_regions;
  city_regions.push_back(&city_region0);
  city_regions.push_back(&city_region1);
  city_regions.push_back(&city_region2);
  city_regions.push_back(&city_region3);
  test_assert("field_region_tests#4", field_region.calculatePoint(&city_regions) == 9);

  delete tile_e0;
  delete tile_e1;
  delete tile_e2;
  delete tile_e3;
  delete tile_e4;
  delete tile_e5;
}

void loop_road_region_tests(TileFactory* tile_factory) {
  Tile* tile0 = tile_factory->newFromName("V", 0);
  Tile* tile1 = tile_factory->newFromName("V", 1);
  Tile* tile2 = tile_factory->newFromName("V", 2);
  Tile* tile3 = tile_factory->newFromName("V", 3);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(0).setY(-1).setRotation(1);
  tile2->setX(-1).setY(-1).setRotation(2);
  tile3->setX(-1).setY(0).setRotation(3);
  RoadRegion region(0, tile0->getRoadSegments()->at(0), nullptr);
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

void loop_road_region_with_cross_road_tests(TileFactory* tile_factory) {
  Tile* tile0 = tile_factory->newFromName("W", 0);
  Tile* tile1 = tile_factory->newFromName("V", 1);
  Tile* tile2 = tile_factory->newFromName("V", 2);
  Tile* tile3 = tile_factory->newFromName("V", 3);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(0).setY(-1).setRotation(1);
  tile2->setX(-1).setY(-1).setRotation(2);
  tile3->setX(-1).setY(0).setRotation(3);
  RoadRegion region(0, tile0->getRoadSegments()->at(1), nullptr);
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

void non_loop_road_region_tests(TileFactory* tile_factory) {
  Tile* tile0 = tile_factory->newFromName("A", 0);
  Tile* tile1 = tile_factory->newFromName("U", 1);
  Tile* tile2 = tile_factory->newFromName("X", 2);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(0).setY(-1).setRotation(0);
  tile2->setX(0).setY(-2).setRotation(0);
  RoadRegion region(0, tile0->getRoadSegments()->at(0), nullptr);
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
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  loop_road_region_tests(&tile_factory);
  loop_road_region_with_cross_road_tests(&tile_factory);
  non_loop_road_region_tests(&tile_factory);
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

template <typename R>
int count_unmerged_region(const std::vector<R*>* regions) {
  int region_count = 0;
  for (auto it = regions->begin(); it != regions->end(); ++it) {
    R* r = *it;
    if (!r->isMerged()) {
      region_count++;
    }
  }
  return region_count;
}

void board_tests() {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  Board board(20, 7);
  board.registerMeeple(MeepleColor::RED);
  board.registerMeeple(MeepleColor::GREEN);
  std::vector<Segment*> meeple_place_candidates;

  Tile* tile = nullptr;
  Tile* tile0 = tile_factory.newFromName("D", 0);
  Tile* tile1 = tile_factory.newFromName("E", 1);
  Tile* tile2 = tile_factory.newFromName("C", 2);
  Tile* tile3 = tile_factory.newFromName("B", 3);
  Tile* tile4 = tile_factory.newFromName("U", 4);

  // turn 0
  board.setInitialTile(tile0, 3);
  test_assert("board_tests#0.0", count_unmerged_region<CityRegion>(board.getCityRegions()) == 1);
  test_assert("board_tests#0.1", count_unmerged_region<CloisterRegion>(board.getCloisterRegions()) == 0);
  test_assert("board_tests#0.2", count_unmerged_region<FieldRegion>(board.getFieldRegions()) == 2);
  test_assert("board_tests#0.3", count_unmerged_region<RoadRegion>(board.getRoadRegions()) == 1);
  test_assert("board_tests#0.4", board.getGameContext()->getTotalPoint(MeepleColor::RED) == 0);
  test_assert("board_tests#0.5", board.getGameContext()->getTotalPoint(MeepleColor::GREEN) == 0);
  test_assert("board_tests#0.6", board.getGameContext()->getHoldingMeepleCount(MeepleColor::RED) == 7);
  test_assert("board_tests#0.7", board.getGameContext()->getHoldingMeepleCount(MeepleColor::GREEN) == 7);

  // turn 1
  test_assert("board_tests#1.0", board.canPlaceTile(tile1, 0, 1, 2));
  board.placeTile(tile1, 0, 1, 2, &meeple_place_candidates);
  test_assert("board_tests#1.1", meeple_place_candidates.size() == 2);
  meeple_place_candidates.clear();
  board.placeMeeple(tile1->getCitySegments()->at(0), MeepleColor::RED);
  board.endTurn();
  test_assert("board_tests#1.2", count_unmerged_region<CityRegion>(board.getCityRegions()) == 1);
  test_assert("board_tests#1.3", count_unmerged_region<CloisterRegion>(board.getCloisterRegions()) == 0);
  test_assert("board_tests#1.4", count_unmerged_region<FieldRegion>(board.getFieldRegions()) == 3);
  test_assert("board_tests#1.5", count_unmerged_region<RoadRegion>(board.getRoadRegions()) == 1);
  test_assert("board_tests#1.6", board.getGameContext()->getTotalPoint(MeepleColor::RED) == 4);
  test_assert("board_tests#1.7", board.getGameContext()->getTotalPoint(MeepleColor::GREEN) == 0);
  test_assert("board_tests#1.8", board.getGameContext()->getHoldingMeepleCount(MeepleColor::RED) == 7);
  test_assert("board_tests#1.9", board.getGameContext()->getHoldingMeepleCount(MeepleColor::GREEN) == 7);

  // turn 2(skip)
  test_assert("board_tests#2.0", !board.hasPossiblePlacement(tile2));

  // turn 3
  test_assert("board_tests#3.0", board.canPlaceTile(tile3, 1, 1, 0));
  board.placeTile(tile3, 1, 1, 0, &meeple_place_candidates);
  test_assert("board_tests#3.1", meeple_place_candidates.size() == 2);
  meeple_place_candidates.clear();
  board.placeMeeple(tile3->getCloisterSegment(), MeepleColor::GREEN);
  board.endTurn();
  test_assert("board_tests#3.2", count_unmerged_region<CityRegion>(board.getCityRegions()) == 1);
  test_assert("board_tests#3.3", count_unmerged_region<CloisterRegion>(board.getCloisterRegions()) == 1);
  test_assert("board_tests#3.4", count_unmerged_region<FieldRegion>(board.getFieldRegions()) == 3);
  test_assert("board_tests#3.5", count_unmerged_region<RoadRegion>(board.getRoadRegions()) == 1);
  test_assert("board_tests#3.6", board.getGameContext()->getTotalPoint(MeepleColor::RED) == 4);
  test_assert("board_tests#3.7", board.getGameContext()->getTotalPoint(MeepleColor::GREEN) == 0);
  test_assert("board_tests#3.8", board.getGameContext()->getHoldingMeepleCount(MeepleColor::RED) == 7);
  test_assert("board_tests#3.9", board.getGameContext()->getHoldingMeepleCount(MeepleColor::GREEN) == 6);

  // turn 4
  test_assert("board_tests#4.0", board.canPlaceTile(tile4, -1, 0, 1));
  board.placeTile(tile4, -1, 0, 1, &meeple_place_candidates);
  board.endTurn();
  test_assert("board_tests#4.1", meeple_place_candidates.size() == 3);
  meeple_place_candidates.clear();
  board.endTurn();
  test_assert("board_tests#4.2", count_unmerged_region<CityRegion>(board.getCityRegions()) == 1);
  test_assert("board_tests#4.3", count_unmerged_region<CloisterRegion>(board.getCloisterRegions()) == 1);
  test_assert("board_tests#4.4", count_unmerged_region<FieldRegion>(board.getFieldRegions()) == 3);
  test_assert("board_tests#4.5", count_unmerged_region<RoadRegion>(board.getRoadRegions()) == 1);
  test_assert("board_tests#4.6", board.getGameContext()->getTotalPoint(MeepleColor::RED) == 4);
  test_assert("board_tests#4.7", board.getGameContext()->getTotalPoint(MeepleColor::GREEN) == 0);
  test_assert("board_tests#4.8", board.getGameContext()->getHoldingMeepleCount(MeepleColor::RED) == 7);
  test_assert("board_tests#4.9", board.getGameContext()->getHoldingMeepleCount(MeepleColor::GREEN) == 6);

  test_assert("board_tests#5.0", board.isUndoable());
  tile = board.undo(); // turn4を取り消し
  test_assert("board_tests#5.1", tile == tile4);
  test_assert("board_tests#5.2", board.isUndoable());
  tile = board.undo(); // turn3を取り消し
  test_assert("board_tests#5.3", tile == tile3);
  test_assert("board_tests#5.4", board.isUndoable());
  tile = board.undo(); // turn1を取り消し
  test_assert("board_tests#5.5", !board.isUndoable());

  test_assert("board_tests#6.0", count_unmerged_region<CityRegion>(board.getCityRegions()) == 1);
  test_assert("board_tests#6.1", count_unmerged_region<CloisterRegion>(board.getCloisterRegions()) == 0);
  test_assert("board_tests#6.2", count_unmerged_region<FieldRegion>(board.getFieldRegions()) == 2);
  test_assert("board_tests#6.3", count_unmerged_region<RoadRegion>(board.getRoadRegions()) == 1);
  test_assert("board_tests#6.4", board.getGameContext()->getTotalPoint(MeepleColor::RED) == 0);
  test_assert("board_tests#6.5", board.getGameContext()->getTotalPoint(MeepleColor::GREEN) == 0);
  test_assert("board_tests#6.6", board.getGameContext()->getHoldingMeepleCount(MeepleColor::RED) == 7);
  test_assert("board_tests#6.7", board.getGameContext()->getHoldingMeepleCount(MeepleColor::GREEN) == 7);

  delete tile0;
  delete tile1;
  delete tile2;
  delete tile3;
  delete tile4;
}

void cloister_completion_undo_board_tests() {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  Board board(20, 7);
  board.registerMeeple(MeepleColor::RED);
  board.registerMeeple(MeepleColor::GREEN);
  std::vector<Segment*> meeple_place_candidates;
  Tile* tile0 = tile_factory.newFromName("B", 0);
  Tile* tile1 = tile_factory.newFromName("B", 1);
  Tile* tile2 = tile_factory.newFromName("B", 2);
  Tile* tile3 = tile_factory.newFromName("B", 3);
  Tile* tile4 = tile_factory.newFromName("B", 4);
  Tile* tile5 = tile_factory.newFromName("B", 5);
  Tile* tile6 = tile_factory.newFromName("B", 6);
  Tile* tile7 = tile_factory.newFromName("B", 7);
  Tile* tile8 = tile_factory.newFromName("B", 8);

  board.setInitialTile(tile0);
  board.placeTile(tile1,  1,  0, 0, &meeple_place_candidates); meeple_place_candidates.clear();
  board.placeTile(tile2, -1,  0, 0, &meeple_place_candidates); meeple_place_candidates.clear();
  board.placeTile(tile3, -1,  1, 0, &meeple_place_candidates); meeple_place_candidates.clear();
  board.placeTile(tile4,  0,  1, 0, &meeple_place_candidates); meeple_place_candidates.clear();
  board.placeTile(tile5,  1,  1, 0, &meeple_place_candidates); meeple_place_candidates.clear();
  board.placeTile(tile6, -1, -1, 0, &meeple_place_candidates); meeple_place_candidates.clear();
  board.placeTile(tile7,  0, -1, 0, &meeple_place_candidates); meeple_place_candidates.clear();
  Region* r = tile0->getCloisterSegment()->getRegion();
  test_assert("cloister_completion_undo_board_tests#0", !r->isCompleted());
  board.placeTile(tile8, 1, -1, 0, &meeple_place_candidates); meeple_place_candidates.clear();
  test_assert("cloister_completion_undo_board_tests#1", r->isCompleted());
  board.undo();
  test_assert("cloister_completion_undo_board_tests#2", !r->isCompleted());

  delete tile0;
  delete tile1;
  delete tile2;
  delete tile3;
  delete tile4;
  delete tile5;
  delete tile6;
  delete tile7;
  delete tile8;
}

// https://github.com/sash2104/carcassonne/issues/14
// のバグ修正確認のためのテスト
void undo_board_tests() {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  Board board(20, 7);
  board.registerMeeple(MeepleColor::RED);
  board.registerMeeple(MeepleColor::GREEN);
  std::vector<Segment*> meeple_place_candidates;
  Tile* tile_q = tile_factory.newFromName("Q", 0);
  Tile* tile_j = tile_factory.newFromName("J", 1);
  Tile* tile_w = tile_factory.newFromName("W", 2);
  Tile* tile_m = tile_factory.newFromName("M", 3);
  Tile* tile_v = tile_factory.newFromName("V", 4);
  Tile* tile_b = tile_factory.newFromName("B", 5);
  Tile* tile_a = tile_factory.newFromName("A", 6);
  Tile* tile_u = tile_factory.newFromName("U", 7);
  Tile* tile_k = tile_factory.newFromName("K", 8);

  board.setInitialTile(tile_q, 2);
  test_assert("undo_board_tests#0", board.canPlaceTile(tile_j, 1, 0, 3));
  board.placeTile(tile_j, 1,  0, 3, &meeple_place_candidates); meeple_place_candidates.clear();
  test_assert("undo_board_tests#1", board.canPlaceTile(tile_w, 2, 0, 2));
  board.placeTile(tile_w, 2,  0, 2, &meeple_place_candidates); meeple_place_candidates.clear();
  test_assert("undo_board_tests#2", board.canPlaceTile(tile_m, 0, -1, 1));
  board.placeTile(tile_m, 0, -1, 1, &meeple_place_candidates); meeple_place_candidates.clear();
  test_assert("undo_board_tests#3", board.canPlaceTile(tile_v, 2, -1, 0));
  board.placeTile(tile_v, 2, -1, 0, &meeple_place_candidates); meeple_place_candidates.clear();
  test_assert("undo_board_tests#4", board.canPlaceTile(tile_b, 0, -2, 0));
  board.placeTile(tile_b, 0, -2, 0, &meeple_place_candidates); meeple_place_candidates.clear();
  test_assert("undo_board_tests#5", board.canPlaceTile(tile_a, 1, -2, 2));
  board.placeTile(tile_a, 1, -2, 2, &meeple_place_candidates); meeple_place_candidates.clear();
  test_assert("undo_board_tests#6", board.canPlaceTile(tile_u, 2, -2, 0));
  board.placeTile(tile_u, 2, -2, 0, &meeple_place_candidates); meeple_place_candidates.clear();
  test_assert("undo_board_tests#7", board.canPlaceTile(tile_k, 1, -1, 2));
  board.placeTile(tile_k, 1, -1, 2, &meeple_place_candidates); meeple_place_candidates.clear();
  test_assert("undo_board_tests#8", board.isUndoable());
  board.undo();

  delete tile_q;
  delete tile_j;
  delete tile_w;
  delete tile_m;
  delete tile_v;
  delete tile_b;
  delete tile_a;
  delete tile_u;
  delete tile_k;
}

void tests() {
  tile_position_map_tests();
  tile_tests();
  segment_tests();
  region_iterator_tests();
  cloister_region_tests();
  city_region_tests();
  field_region_tests();
  road_region_tests();
  game_context_tests();
  board_tests();
  cloister_completion_undo_board_tests();
  undo_board_tests();
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
