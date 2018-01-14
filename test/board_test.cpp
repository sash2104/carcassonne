#include <vector>

#include "board.hpp"
#include "game_context.hpp"
#include "meeple_color.hpp"
#include "region.hpp"
#include "segment.hpp"
#include "tile.hpp"
#include "tile_factory.hpp"
#include "gtest/gtest.h"

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

TEST(Board, Basic) {
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

  // turn0
  board.setInitialTile(tile0, 3);
  EXPECT_EQ(1, count_unmerged_region<CityRegion>(board.getCityRegions()));
  EXPECT_EQ(0, count_unmerged_region<CloisterRegion>(board.getCloisterRegions()));
  EXPECT_EQ(2, count_unmerged_region<FieldRegion>(board.getFieldRegions()));
  EXPECT_EQ(1, count_unmerged_region<RoadRegion>(board.getRoadRegions()));
  EXPECT_EQ(0, board.getGameContext()->getTotalPoint(MeepleColor::RED));
  EXPECT_EQ(0, board.getGameContext()->getTotalPoint(MeepleColor::GREEN));
  EXPECT_EQ(7, board.getGameContext()->getHoldingMeepleCount(MeepleColor::RED));
  EXPECT_EQ(7, board.getGameContext()->getHoldingMeepleCount(MeepleColor::GREEN));

  // turn1
  ASSERT_TRUE(board.canPlaceTile(tile1, 0, 1, 2));
  board.placeTile(tile1, 0, 1, 2, &meeple_place_candidates);
  EXPECT_EQ(2, meeple_place_candidates.size());
  meeple_place_candidates.clear();
  board.placeMeeple(tile1->getCitySegments()->at(0), MeepleColor::RED);
  board.endTurn();
  EXPECT_EQ(1, count_unmerged_region<CityRegion>(board.getCityRegions()));
  EXPECT_EQ(0, count_unmerged_region<CloisterRegion>(board.getCloisterRegions()));
  EXPECT_EQ(3, count_unmerged_region<FieldRegion>(board.getFieldRegions()));
  EXPECT_EQ(1, count_unmerged_region<RoadRegion>(board.getRoadRegions()));
  EXPECT_EQ(4, board.getGameContext()->getTotalPoint(MeepleColor::RED));
  EXPECT_EQ(0, board.getGameContext()->getTotalPoint(MeepleColor::GREEN));
  EXPECT_EQ(7, board.getGameContext()->getHoldingMeepleCount(MeepleColor::RED));
  EXPECT_EQ(7, board.getGameContext()->getHoldingMeepleCount(MeepleColor::GREEN));

  // turn2(skip)
  ASSERT_FALSE(board.hasPossiblePlacement(tile2));

  // turn3
  ASSERT_TRUE(board.canPlaceTile(tile3, 1, 1, 0));
  board.placeTile(tile3, 1, 1, 0, &meeple_place_candidates);
  EXPECT_EQ(2, meeple_place_candidates.size());
  meeple_place_candidates.clear();
  board.placeMeeple(tile3->getCloisterSegment(), MeepleColor::GREEN);
  board.endTurn();
  EXPECT_EQ(1, count_unmerged_region<CityRegion>(board.getCityRegions()));
  EXPECT_EQ(1, count_unmerged_region<CloisterRegion>(board.getCloisterRegions()));
  EXPECT_EQ(3, count_unmerged_region<FieldRegion>(board.getFieldRegions()));
  EXPECT_EQ(1, count_unmerged_region<RoadRegion>(board.getRoadRegions()));
  EXPECT_EQ(4, board.getGameContext()->getTotalPoint(MeepleColor::RED));
  EXPECT_EQ(0, board.getGameContext()->getTotalPoint(MeepleColor::GREEN));
  EXPECT_EQ(7, board.getGameContext()->getHoldingMeepleCount(MeepleColor::RED));
  EXPECT_EQ(6, board.getGameContext()->getHoldingMeepleCount(MeepleColor::GREEN));

  // turn4
  ASSERT_TRUE(board.canPlaceTile(tile4, -1, 0, 1));
  board.placeTile(tile4, -1, 0, 1, &meeple_place_candidates);
  EXPECT_EQ(3, meeple_place_candidates.size());
  meeple_place_candidates.clear();
  board.endTurn();
  EXPECT_EQ(1, count_unmerged_region<CityRegion>(board.getCityRegions()));
  EXPECT_EQ(1, count_unmerged_region<CloisterRegion>(board.getCloisterRegions()));
  EXPECT_EQ(3, count_unmerged_region<FieldRegion>(board.getFieldRegions()));
  EXPECT_EQ(1, count_unmerged_region<RoadRegion>(board.getRoadRegions()));
  EXPECT_EQ(4, board.getGameContext()->getTotalPoint(MeepleColor::RED));
  EXPECT_EQ(0, board.getGameContext()->getTotalPoint(MeepleColor::GREEN));
  EXPECT_EQ(7, board.getGameContext()->getHoldingMeepleCount(MeepleColor::RED));
  EXPECT_EQ(6, board.getGameContext()->getHoldingMeepleCount(MeepleColor::GREEN));

  ASSERT_TRUE(board.isUndoable());
  tile = board.undo(); // turn4を取り消し
  EXPECT_EQ(tile4, tile);
  ASSERT_TRUE(board.isUndoable());
  tile = board.undo(); // turn3を取り消し
  EXPECT_EQ(tile3, tile);
  ASSERT_TRUE(board.isUndoable());
  tile = board.undo(); // turn1を取り消し
  EXPECT_EQ(tile1, tile);
  ASSERT_FALSE(board.isUndoable());

  EXPECT_EQ(1, count_unmerged_region<CityRegion>(board.getCityRegions()));
  EXPECT_EQ(0, count_unmerged_region<CloisterRegion>(board.getCloisterRegions()));
  EXPECT_EQ(2, count_unmerged_region<FieldRegion>(board.getFieldRegions()));
  EXPECT_EQ(1, count_unmerged_region<RoadRegion>(board.getRoadRegions()));
  EXPECT_EQ(0, board.getGameContext()->getTotalPoint(MeepleColor::RED));
  EXPECT_EQ(0, board.getGameContext()->getTotalPoint(MeepleColor::GREEN));
  EXPECT_EQ(7, board.getGameContext()->getHoldingMeepleCount(MeepleColor::RED));
  EXPECT_EQ(7, board.getGameContext()->getHoldingMeepleCount(MeepleColor::GREEN));

  delete tile0;
  delete tile1;
  delete tile2;
  delete tile3;
  delete tile4;
}

TEST(Board, ColoisterRegionCompletionUndo) {
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
  EXPECT_FALSE(r->isCompleted());
  board.placeTile(tile8, 1, -1, 0, &meeple_place_candidates); meeple_place_candidates.clear();
  EXPECT_TRUE(r->isCompleted());
  board.undo();
  EXPECT_FALSE(r->isCompleted());

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
TEST(Board, Undo) {
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
  ASSERT_TRUE(board.canPlaceTile(tile_j, 1, 0, 3));
  board.placeTile(tile_j, 1,  0, 3, &meeple_place_candidates); meeple_place_candidates.clear();
  ASSERT_TRUE(board.canPlaceTile(tile_w, 2, 0, 2));
  board.placeTile(tile_w, 2,  0, 2, &meeple_place_candidates); meeple_place_candidates.clear();
  ASSERT_TRUE(board.canPlaceTile(tile_m, 0, -1, 1));
  board.placeTile(tile_m, 0, -1, 1, &meeple_place_candidates); meeple_place_candidates.clear();
  ASSERT_TRUE(board.canPlaceTile(tile_v, 2, -1, 0));
  board.placeTile(tile_v, 2, -1, 0, &meeple_place_candidates); meeple_place_candidates.clear();
  ASSERT_TRUE(board.canPlaceTile(tile_b, 0, -2, 0));
  board.placeTile(tile_b, 0, -2, 0, &meeple_place_candidates); meeple_place_candidates.clear();
  ASSERT_TRUE(board.canPlaceTile(tile_a, 1, -2, 2));
  board.placeTile(tile_a, 1, -2, 2, &meeple_place_candidates); meeple_place_candidates.clear();
  ASSERT_TRUE(board.canPlaceTile(tile_u, 2, -2, 0));
  board.placeTile(tile_u, 2, -2, 0, &meeple_place_candidates); meeple_place_candidates.clear();
  ASSERT_TRUE(board.canPlaceTile(tile_k, 1, -1, 2));
  board.placeTile(tile_k, 1, -1, 2, &meeple_place_candidates); meeple_place_candidates.clear();
  ASSERT_TRUE(board.isUndoable());
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
