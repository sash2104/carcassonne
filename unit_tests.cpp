#include <cstdlib>
#include <set>

#include "test_util.hpp"
#include "tile.hpp"
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

void tests() {
  tile_position_map_tests();
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
