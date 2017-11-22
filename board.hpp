#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <array>
#include <string>

#include "meeple_color.hpp"
#include "region.hpp"
#include "segment.hpp"
#include "tile.hpp"
#include "tile_holder.hpp"
#include "tile_position_map.hpp"

class Region;
class Segment;
class CityRegion;
class CloisterRegion;
class FieldRegion;
class LargeTileHolder;
class RoadRegion;

class Board {
  public:
    Board(int tile_n);
    ~Board();

  static const int N_TILES = 72;
  static const int FIELD_SIZE = 441;

    const Tile* getCurrentTile(int turn) const;
    int getFieldPosId(int x, int y);
    void setPile(const std::vector<Tile*> &tiles);
    void placeTile(const Tile* tile, int dir, int x, int y); // field_の(x, y)にタイルをdirの向きに配置
    void printField(); // LargeTileHolderが埋まった位置のpos_idのリストを出力

  const TilePositionMap* getTilePositionMap() const;
  const std::vector<CityRegion*>* getCityRegions() const;
  const std::vector<CloisterRegion*>* getCloisterRegions() const;
  const std::vector<FieldRegion*>* getFieldRegions() const;
  const std::vector<RoadRegion*>* getRoadRegions() const;
  bool canPlaceTile(Tile* tile, int x, int y, int rotation);
  bool hasPossiblePlacement(Tile* tile);
  bool isAdjacencyValid(Tile* tile, int x, int y, int rotation);
  void setInitialTile(Tile* tile);
  void setInitialTile(Tile* tile, int rotation);
  bool placeTile(Tile* tile, int x, int y, int rotation, std::vector<Segment*>* meeplePlaceCandidates);
  bool placeMeeple(Segment* segment, MeepleColor meeple);
  private:
    std::array<const Tile*, N_TILES> pile_; // 未配置のタイルを管理
    std::array<LargeTileHolder*, FIELD_SIZE> field_; // 配置済のタイルを管理
    int num_remain_tiles_;

  void doPlaceTile(Tile* tile, int x, int y, int rotation, std::vector<Segment*>* meeplePlaceCandidates);
  TilePositionMap tile_map_;
  std::vector<CityRegion*> city_regions_;
  std::vector<CloisterRegion*> cloister_regions_;
  std::vector<FieldRegion*> field_regions_;
  std::vector<RoadRegion*> road_regions_;
};

#endif
