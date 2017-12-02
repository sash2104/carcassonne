#include <cstdlib>
#include <set>

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

void cup_city_tests(TileFactory* tile_f) {
  Tile* tile01 = tile_f->newFromName("E", 0);
  Tile* tile02 = tile_f->newFromName("E", 1);
  tile01->setX(0).setY(0).setRotation(2);
  tile02->setX(0).setY(-1).setRotation(0);
  CityRegion region(0, nullptr);
  region.addSegment(tile01->getCitySegments()->at(0));
  test_assert("cup_city_tests#0", !region.isCompleted());
  test_assert("cup_city_tests#1", region.calculatePoint() == 1);
  region.addSegment(tile02->getCitySegments()->at(0));
  test_assert("cup_city_tests#2", region.isCompleted());
  test_assert("cup_city_tests#3", region.calculatePoint() == 4);
  delete tile01;
  delete tile02;
}

void city_region_tests() {
  TileFactory tile_f;
  tile_f.loadResource("tiles.json");
  cup_city_tests(&tile_f);
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

void tests() {
  tile_position_map_tests();
  cloister_region_tests();
  city_region_tests();
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
