#include "game_context.hpp"
#include "meeple_color.hpp"
#include "region.hpp"
#include "gtest/gtest.h"

TEST(GameContextTest, Basic) {
  GameContext context(7);
  context.registerMeeple(MeepleColor::RED);
  context.registerMeeple(MeepleColor::GREEN);

  context.placeMeeple(MeepleColor::RED);
  context.placeMeeple(MeepleColor::RED);
  context.placeMeeple(MeepleColor::GREEN);
  context.placeMeeple(MeepleColor::GREEN);
  context.placeMeeple(MeepleColor::GREEN);
  EXPECT_EQ(5, context.getHoldingMeepleCount(MeepleColor::RED));
  EXPECT_EQ(2, context.getOnBoardMeepleCount(MeepleColor::RED));
  EXPECT_EQ(0, context.getReturnedMeepleCount(MeepleColor::RED));
  EXPECT_EQ(4, context.getHoldingMeepleCount(MeepleColor::GREEN));
  EXPECT_EQ(3, context.getOnBoardMeepleCount(MeepleColor::GREEN));
  EXPECT_EQ(0, context.getReturnedMeepleCount(MeepleColor::GREEN));

  context.addPoint(MeepleColor::RED, RegionType::CITY, 4);
  context.returnMeeple(MeepleColor::RED, 1);
  context.addPoint(MeepleColor::RED, RegionType::CLOISTER, 9);
  context.returnMeeple(MeepleColor::RED, 1);
  context.addPoint(MeepleColor::GREEN, RegionType::ROAD, 3);
  context.returnMeeple(MeepleColor::GREEN, 1);
  EXPECT_EQ(5, context.getHoldingMeepleCount(MeepleColor::RED));
  EXPECT_EQ(0, context.getOnBoardMeepleCount(MeepleColor::RED));
  EXPECT_EQ(2, context.getReturnedMeepleCount(MeepleColor::RED));
  EXPECT_EQ(4, context.getHoldingMeepleCount(MeepleColor::GREEN));
  EXPECT_EQ(2, context.getOnBoardMeepleCount(MeepleColor::GREEN));
  EXPECT_EQ(1, context.getReturnedMeepleCount(MeepleColor::GREEN));
  EXPECT_EQ(0, context.getTotalPoint(MeepleColor::RED));
  EXPECT_EQ(13, context.getGainedPoint(MeepleColor::RED));
  EXPECT_EQ(4, context.getGainedPoint(MeepleColor::RED, RegionType::CITY));
  EXPECT_EQ(9, context.getGainedPoint(MeepleColor::RED, RegionType::CLOISTER));
  EXPECT_EQ(0, context.getGainedPoint(MeepleColor::RED, RegionType::ROAD));
  EXPECT_EQ(0, context.getGainedPoint(MeepleColor::RED, RegionType::FIELD));
  EXPECT_EQ(0, context.getTotalPoint(MeepleColor::GREEN));
  EXPECT_EQ(3, context.getGainedPoint(MeepleColor::GREEN));
  EXPECT_EQ(0, context.getGainedPoint(MeepleColor::GREEN, RegionType::CITY));
  EXPECT_EQ(0, context.getGainedPoint(MeepleColor::GREEN, RegionType::CLOISTER));
  EXPECT_EQ(3, context.getGainedPoint(MeepleColor::GREEN, RegionType::ROAD));
  EXPECT_EQ(0, context.getGainedPoint(MeepleColor::GREEN, RegionType::FIELD));

  context.endTurn();
  EXPECT_EQ(7, context.getHoldingMeepleCount(MeepleColor::RED));
  EXPECT_EQ(0, context.getOnBoardMeepleCount(MeepleColor::RED));
  EXPECT_EQ(0, context.getReturnedMeepleCount(MeepleColor::RED));
  EXPECT_EQ(5, context.getHoldingMeepleCount(MeepleColor::GREEN));
  EXPECT_EQ(2, context.getOnBoardMeepleCount(MeepleColor::GREEN));
  EXPECT_EQ(0, context.getReturnedMeepleCount(MeepleColor::GREEN));
  EXPECT_EQ(13, context.getTotalPoint(MeepleColor::RED));
  EXPECT_EQ(0, context.getGainedPoint(MeepleColor::RED));
  EXPECT_EQ(0, context.getGainedPoint(MeepleColor::RED, RegionType::CITY));
  EXPECT_EQ(0, context.getGainedPoint(MeepleColor::RED, RegionType::CLOISTER));
  EXPECT_EQ(0, context.getGainedPoint(MeepleColor::RED, RegionType::ROAD));
  EXPECT_EQ(0, context.getGainedPoint(MeepleColor::RED, RegionType::FIELD));
  EXPECT_EQ(3, context.getTotalPoint(MeepleColor::GREEN));
  EXPECT_EQ(0, context.getGainedPoint(MeepleColor::GREEN));
  EXPECT_EQ(0, context.getGainedPoint(MeepleColor::GREEN, RegionType::CITY));
  EXPECT_EQ(0, context.getGainedPoint(MeepleColor::GREEN, RegionType::CLOISTER));
  EXPECT_EQ(0, context.getGainedPoint(MeepleColor::GREEN, RegionType::ROAD));
  EXPECT_EQ(0, context.getGainedPoint(MeepleColor::GREEN, RegionType::FIELD));
}
