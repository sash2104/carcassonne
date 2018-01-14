#include "segment.hpp"
#include "tile.hpp"
#include "tile_factory.hpp"
#include "gtest/gtest.h"

TEST(SegmentTest, isAdjacentTo) {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  Tile* tile_d = tile_factory.newFromName("D", 0);
  Segment* s = tile_d->getCitySegments()->at(0);

  tile_d->setRotation(0);
  EXPECT_TRUE(s->isAdjacentTo(1));
  EXPECT_FALSE(s->isAdjacentTo(3));

  tile_d->setRotation(2);
  EXPECT_FALSE(s->isAdjacentTo(1));
  EXPECT_TRUE(s->isAdjacentTo(3));
  delete tile_d;
}
