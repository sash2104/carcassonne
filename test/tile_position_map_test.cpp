#include "tile.hpp"
#include "tile_position_map.hpp"
#include "gtest/gtest.h"

TEST(TilePositionMapTest, Basic) {
  TilePositionMap m(10);
  Tile tile1(0);
  Tile tile2(1);
  const std::set<BoardPosition>* s;

  EXPECT_FALSE(m.isTilePlaced(0, 0));
  s = m.getPlacablePositions();
  EXPECT_EQ(0, s->size());

  m.placeTile(&tile1, 0, 0);
  EXPECT_TRUE(m.isTilePlaced(0, 0));
  EXPECT_EQ(&tile1, m.getPlacedTile(0, 0));
  s = m.getPlacablePositions();
  EXPECT_EQ(4, s->size());
  EXPECT_FALSE(m.isTilePlaced(1, 0));

  m.placeTile(&tile2, 1, 0);
  EXPECT_EQ(&tile2, m.getPlacedTile(1, 0));
  EXPECT_TRUE(m.isTilePlaced(1, 0));
  s = m.getPlacablePositions();
  EXPECT_EQ(6, s->size());
}

TEST(TilePositionMapTest, Undo) {
  TilePositionMap m(10);
  Tile tile1(0);
  Tile tile2(1);
  Tile* tp;
  const std::set<BoardPosition>* s;

  m.placeTile(&tile1, 0, 0);
  m.placeTile(&tile2, 1, 0);
  s = m.getPlacablePositions();
  EXPECT_EQ(6, s->size());
  EXPECT_TRUE(m.isTilePlaced(0, 0));
  EXPECT_TRUE(m.isTilePlaced(1, 0));
  tp = m.undo(1, 0);
  EXPECT_EQ(&tile2, tp);
  EXPECT_EQ(4, s->size());
  EXPECT_TRUE(m.isTilePlaced(0, 0));
  EXPECT_FALSE(m.isTilePlaced(1, 0));
}
