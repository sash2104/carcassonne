#include <vector>

#include "board.hpp"
#include "region.hpp"
#include "segment.hpp"
#include "tile.hpp"
#include "tile_factory.hpp"
#include "tile_position_map.hpp"
#include "gtest/gtest.h"

int count_region_segments(const Region& region) {
  int count = 0;
  for (auto it = region.segmentBegin(); it != region.segmentEnd(); ++it) {
    count++;
  }
  return count;
}

TEST(RegionTest, SegmentIterator) {
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
  EXPECT_EQ(3, count_region_segments(r0));
  EXPECT_EQ(1, count_region_segments(r1));
  EXPECT_EQ(2, count_region_segments(r2));
  EXPECT_EQ(3, count_region_segments(r3));
  r2.mergeRegion(&r3);
  EXPECT_EQ(5, count_region_segments(r2));
  r0.mergeRegion(&r1);
  EXPECT_EQ(4, count_region_segments(r0));
  r0.mergeRegion(&r2);
  EXPECT_EQ(9, count_region_segments(r0));
  EXPECT_EQ(1, count_region_segments(r1));
  EXPECT_EQ(5, count_region_segments(r2));
  EXPECT_EQ(3, count_region_segments(r3));

  r0.undoMergeRegion(&r2);
  EXPECT_EQ(4, count_region_segments(r0));
  r0.undoMergeRegion(&r1);
  EXPECT_EQ(3, count_region_segments(r0));
}

TEST(CityRegionTest, CupCityRegionCompletion) {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  Tile* tile0 = tile_factory.newFromName("E", 0);
  Tile* tile1 = tile_factory.newFromName("E", 1);
  tile0->setX(0).setY(0).setRotation(2);
  tile1->setX(0).setY(-1).setRotation(0);
  CityRegion region(0, tile0->getCitySegments()->at(0), nullptr);
  EXPECT_FALSE(region.isCompleted());
  EXPECT_EQ(1, region.calculatePoint());
  region.addSegment(tile1->getCitySegments()->at(0));
  EXPECT_TRUE(region.isCompleted());
  EXPECT_EQ(4, region.calculatePoint());

  delete tile0;
  delete tile1;
}

TEST(CityRegionTest, DiamondCityRegionCompletion) {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  Tile* tile0 = tile_factory.newFromName("N", 0);
  Tile* tile1 = tile_factory.newFromName("N", 1);
  Tile* tile2 = tile_factory.newFromName("N", 2);
  Tile* tile3 = tile_factory.newFromName("P", 3);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(-1).setY(0).setRotation(1);
  tile2->setX(-1).setY(1).setRotation(2);
  tile3->setX(0).setY(1).setRotation(3);
  CityRegion region(0, tile0->getCitySegments()->at(0), nullptr);
  region.addSegment(tile1->getCitySegments()->at(0));
  region.addSegment(tile2->getCitySegments()->at(0));
  EXPECT_FALSE(region.isCompleted());
  EXPECT_EQ(3, region.calculatePoint());
  region.addSegment(tile3->getCitySegments()->at(0));
  EXPECT_TRUE(region.isCompleted());
  EXPECT_EQ(8, region.calculatePoint());
  delete tile0;
  delete tile1;
  delete tile2;
  delete tile3;
}

TEST(CityRegionTest, DiamondCityRegionWithOnePennantCompletion) {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  Tile* tile0 = tile_factory.newFromName("N", 0);
  Tile* tile1 = tile_factory.newFromName("N", 1);
  Tile* tile2 = tile_factory.newFromName("M", 2);
  Tile* tile3 = tile_factory.newFromName("P", 3);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(-1).setY(0).setRotation(1);
  tile2->setX(-1).setY(1).setRotation(2);
  tile3->setX(0).setY(1).setRotation(3);
  CityRegion region(0, tile0->getCitySegments()->at(0), nullptr);
  region.addSegment(tile1->getCitySegments()->at(0));
  region.addSegment(tile2->getCitySegments()->at(0));
  EXPECT_FALSE(region.isCompleted());
  EXPECT_EQ(4, region.calculatePoint());
  region.addSegment(tile3->getCitySegments()->at(0));
  EXPECT_TRUE(region.isCompleted());
  EXPECT_EQ(10, region.calculatePoint());
  delete tile0;
  delete tile1;
  delete tile2;
  delete tile3;
}

TEST(CityRegionTest, DiamondCityRegionIncludingITileCompletion) {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  Tile* tile0 = tile_factory.newFromName("N", 0);
  Tile* tile1 = tile_factory.newFromName("N", 1);
  Tile* tile2 = tile_factory.newFromName("I", 2);
  Tile* tile3 = tile_factory.newFromName("P", 3);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(-1).setY(0).setRotation(1);
  tile2->setX(-1).setY(1).setRotation(0);
  tile3->setX(0).setY(1).setRotation(3);
  CityRegion region(0, tile0->getCitySegments()->at(0), nullptr);
  region.addSegment(tile1->getCitySegments()->at(0));
  region.addSegment(tile3->getCitySegments()->at(0));
  EXPECT_FALSE(region.isCompleted());
  EXPECT_EQ(3, region.calculatePoint());
  region.addSegment(tile2->getCitySegments()->at(0));
  region.addSegment(tile2->getCitySegments()->at(1));
  EXPECT_TRUE(region.isCompleted());
  EXPECT_EQ(8, region.calculatePoint());
  delete tile0;
  delete tile1;
  delete tile2;
  delete tile3;
}

TEST(CloisterRegionTest, Completion) {
  Board board(10, 7);
  Segment s(0, SegmentType::CLOISTER, false);
  Tile tile(0);
  tile.setX(0).setY(0).setRotation(0);
  s.setTile(&tile);
  CloisterRegion region(0, &s, &board);
  TilePositionMap* map = const_cast<TilePositionMap*>(board.getTilePositionMap());
  map->placeTile(&tile, 0, 0);
  EXPECT_FALSE(region.isCompleted());
  EXPECT_EQ(1, region.calculatePoint());
  for (int dx = -1; dx < 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (!(dx == 0 && dy == 0)) {
	map->placeTile(&tile, dx, dy);
      }
    }
  }
  EXPECT_FALSE(region.isCompleted());
  EXPECT_EQ(6, region.calculatePoint());
  for (int dy = -1; dy <= 1; dy++) {
    map->placeTile(&tile, 1, dy);
  }
  EXPECT_TRUE(region.isCompleted());
  EXPECT_EQ(9, region.calculatePoint());
}

TEST(FieldRegionTest, Completion) {
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
  EXPECT_TRUE(city_region0.isCompleted());
  EXPECT_TRUE(city_region1.isCompleted());
  EXPECT_TRUE(city_region2.isCompleted());
  EXPECT_FALSE(city_region3.isCompleted());

  std::vector<CityRegion*> city_regions;
  city_regions.push_back(&city_region0);
  city_regions.push_back(&city_region1);
  city_regions.push_back(&city_region2);
  city_regions.push_back(&city_region3);
  EXPECT_EQ(9, field_region.calculatePoint(&city_regions));

  delete tile_e0;
  delete tile_e1;
  delete tile_e2;
  delete tile_e3;
  delete tile_e4;
  delete tile_e5;
}

TEST(RoadRegionTest, LoopRoadRegionCompletion) {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  Tile* tile0 = tile_factory.newFromName("V", 0);
  Tile* tile1 = tile_factory.newFromName("V", 1);
  Tile* tile2 = tile_factory.newFromName("V", 2);
  Tile* tile3 = tile_factory.newFromName("V", 3);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(0).setY(-1).setRotation(1);
  tile2->setX(-1).setY(-1).setRotation(2);
  tile3->setX(-1).setY(0).setRotation(3);
  RoadRegion region(0, tile0->getRoadSegments()->at(0), nullptr);
  region.addSegment(tile1->getRoadSegments()->at(0));
  region.addSegment(tile2->getRoadSegments()->at(0));
  EXPECT_FALSE(region.isCompleted());
  EXPECT_EQ(3, region.calculatePoint());
  region.addSegment(tile3->getRoadSegments()->at(0));
  EXPECT_TRUE(region.isCompleted());
  EXPECT_EQ(4, region.calculatePoint());
  delete tile0;
  delete tile1;
  delete tile2;
  delete tile3;
}

TEST(RoadRegionTest, LoopRoadRegionWithCrossRoadCompletion) {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  Tile* tile0 = tile_factory.newFromName("W", 0);
  Tile* tile1 = tile_factory.newFromName("V", 1);
  Tile* tile2 = tile_factory.newFromName("V", 2);
  Tile* tile3 = tile_factory.newFromName("V", 3);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(0).setY(-1).setRotation(1);
  tile2->setX(-1).setY(-1).setRotation(2);
  tile3->setX(-1).setY(0).setRotation(3);
  RoadRegion region(0, tile0->getRoadSegments()->at(1), nullptr);
  region.addSegment(tile1->getRoadSegments()->at(0));
  region.addSegment(tile2->getRoadSegments()->at(0));
  EXPECT_FALSE(region.isCompleted());
  EXPECT_EQ(3, region.calculatePoint());
  region.addSegment(tile0->getRoadSegments()->at(2));
  region.addSegment(tile3->getRoadSegments()->at(0));
  EXPECT_TRUE(region.isCompleted());
  EXPECT_EQ(4, region.calculatePoint());
  delete tile0;
  delete tile1;
  delete tile2;
  delete tile3;
}

TEST(RoadRegionTest, NonLoopRoadRegionCompletion) {
  TileFactory tile_factory;
  tile_factory.loadResource("tiles.json");
  Tile* tile0 = tile_factory.newFromName("A", 0);
  Tile* tile1 = tile_factory.newFromName("U", 1);
  Tile* tile2 = tile_factory.newFromName("X", 2);
  tile0->setX(0).setY(0).setRotation(0);
  tile1->setX(0).setY(-1).setRotation(0);
  tile2->setX(0).setY(-2).setRotation(0);
  RoadRegion region(0, tile0->getRoadSegments()->at(0), nullptr);
  region.addSegment(tile1->getRoadSegments()->at(0));
  EXPECT_FALSE(region.isCompleted());
  EXPECT_EQ(2, region.calculatePoint());
  region.addSegment(tile2->getRoadSegments()->at(0));
  EXPECT_TRUE(region.isCompleted());
  EXPECT_EQ(3, region.calculatePoint());
  delete tile0;
  delete tile1;
  delete tile2;
}
