#include "tile.hpp"
#include "tile_factory.hpp"
#include "gtest/gtest.h"

TEST(TileTest, getBorderType) {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  Tile* tile_d = tile_factory.newFromName("D", 0);

  tile_d->setRotation(0);
  EXPECT_EQ(BorderType::ROAD, tile_d->getBorderType(0));
  EXPECT_EQ(BorderType::CITY, tile_d->getBorderType(1));
  EXPECT_EQ(BorderType::ROAD, tile_d->getBorderType(2));
  EXPECT_EQ(BorderType::FIELD, tile_d->getBorderType(3));

  tile_d->setRotation(3);
  EXPECT_EQ(BorderType::CITY, tile_d->getBorderType(0));
  EXPECT_EQ(BorderType::ROAD, tile_d->getBorderType(1));
  EXPECT_EQ(BorderType::FIELD, tile_d->getBorderType(2));
  EXPECT_EQ(BorderType::ROAD, tile_d->getBorderType(3));

  delete tile_d;
}

TEST(TileTest, canAdjacentWith) {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  Tile* tile_d = tile_factory.newFromName("D", 0);
  Tile* tile_j = tile_factory.newFromName("J", 1);

  tile_d->setRotation(0);
  EXPECT_TRUE(tile_d->canAdjacentWith(0, tile_j, 0));
  EXPECT_FALSE(tile_d->canAdjacentWith(0, tile_j, 2));
  EXPECT_FALSE(tile_d->canAdjacentWith(3, tile_j, 0));
  EXPECT_TRUE(tile_d->canAdjacentWith(3, tile_j, 2));

  tile_d->setRotation(2);
  EXPECT_TRUE(tile_d->canAdjacentWith(0, tile_j, 0));
  EXPECT_FALSE(tile_d->canAdjacentWith(0, tile_j, 2));
  EXPECT_FALSE(tile_d->canAdjacentWith(3, tile_j, 0));
  EXPECT_FALSE(tile_d->canAdjacentWith(3, tile_j, 2));

  delete tile_d;
  delete tile_j;
}

TEST(TileTest, isTwoSegmentAdjacent) {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  Tile* tile_j = tile_factory.newFromName("J", 1);

  EXPECT_TRUE(tile_j->isTwoSegmentAdjacent(0, 0));
  EXPECT_FALSE(tile_j->isTwoSegmentAdjacent(1, 0));

  delete tile_j;
}
