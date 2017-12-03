#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <string>

#include "game_context.hpp"
#include "meeple_color.hpp"
#include "region.hpp"
#include "segment.hpp"
#include "tile.hpp"
#include "tile_position_map.hpp"

class Region;
class Segment;
class CityRegion;
class CloisterRegion;
class FieldRegion;
class GameContext;
class LargeTileHolder;
class RoadRegion;

class Board {
  public:
    Board(int tile_n);
    ~Board();
    TilePositionMap* getTilePositionMap();
    const std::vector<CityRegion*>* getCityRegions() const;
    const std::vector<CloisterRegion*>* getCloisterRegions() const;
    const std::vector<FieldRegion*>* getFieldRegions() const;
    const std::vector<RoadRegion*>* getRoadRegions() const;
    bool canPlaceTile(Tile* tile, int x, int y, int rotation);
    bool hasPossiblePlacement(Tile* tile);
    bool adjacencyIsValid(Tile* tile, int x, int y, int rotation);
    void setInitialTile(Tile* tile);
    void setInitialTile(Tile* tile, int rotation);
    bool placeTile(Tile* tile, int x, int y, int rotation,
      std::vector<Segment*>* meeplePlaceCandidates, GameContext* context);
    bool placeMeeple(Segment* segment, MeepleColor color, GameContext* context);
    void transferRemainingPoints(GameContext* context, bool returnMeeple);
  private:
    int region_id_;
    TilePositionMap tile_map_;
    std::vector<Tile*> placed_tiles_;
    std::vector<CityRegion*> city_regions_;
    std::vector<CloisterRegion*> cloister_regions_;
    std::vector<FieldRegion*> field_regions_;
    std::vector<RoadRegion*> road_regions_;
};

#endif
