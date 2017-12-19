#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <string>
#include <vector>

#include "game_context.hpp"
#include "meeple_color.hpp"
#include "tile_position_map.hpp"

enum class MeepleColor;
class Tile;
class Segment;
class Region;
class CityRegion;
class CloisterRegion;
class FieldRegion;
class RoadRegion;
class GameContext;

class MeeplePlacementEvent {
  public:
    MeeplePlacementEvent(Segment* s) : s_(s) {};
    ~MeeplePlacementEvent() {};
    Segment* getSegment() const {
      return s_;
    };
  private:
    Segment* s_;
};

class RegionCompositionEvent {
  public:
    RegionCompositionEvent(Segment* added_segment)
      : added_segment_(added_segment),  merged_regions_() {};
    ~RegionCompositionEvent() {};
    void addMergedRegion(Region* region) {
      merged_regions_.push_back(region);
    };
    Segment* getAddedSegment() const {
      return added_segment_;
    };
    std::vector<Region*>* getMergedRegions() {
      return &merged_regions_;
    };
  private:
    Segment* added_segment_;
    std::vector<Region*> merged_regions_;
};

class TilePlacementEvent {
  public:
    TilePlacementEvent(Tile* tile)
      : tile_(tile), generated_regions_(), events_(), completed_regions_() {};
    ~TilePlacementEvent() {
      for (auto it = events_.begin(); it != events_.end();) {
	RegionCompositionEvent* event = *it;
	it = events_.erase(it);
	delete event;
      }
    };
    Tile* getTile() const {
      return tile_;
    };
    std::vector<Region*>* getGeneratedRegions() {
      return &generated_regions_;
    };
    std::vector<RegionCompositionEvent*>* getRegionCompositionEvents() {
      return &events_;
    };
    std::vector<Region*>* getCompletedRegions() {
      return &completed_regions_;
    };
    void addGeneratedRegion(Region* region) {
      generated_regions_.push_back(region);
    };
    void addRegionCompositionEvent(RegionCompositionEvent* event) {
      events_.push_back(event);
    };
    void addCompletedRegion(Region* region) {
      completed_regions_.push_back(region);
    };
  private:
    Tile* tile_;
    std::vector<Region*> generated_regions_;
    std::vector<RegionCompositionEvent*> events_;
    std::vector<Region*> completed_regions_;
};


class PlacementEvent {
  public:
    PlacementEvent(TilePlacementEvent* tile_event)
      : tile_event_(tile_event), meeple_event_(nullptr) {};
    ~PlacementEvent() {
      delete tile_event_;
      if (meeple_event_ != nullptr) {
	delete meeple_event_;
      }
    };
    TilePlacementEvent* getTilePlacementEvent() const {
      return tile_event_;
    };
    MeeplePlacementEvent* getMeeplePlacementEvent() const {
      return meeple_event_;
    };
    void setMeeplePlacementEvent(MeeplePlacementEvent* meeple_event) {
      meeple_event_ = meeple_event;
    };
  private:
    TilePlacementEvent* tile_event_;
    MeeplePlacementEvent* meeple_event_;
};

class Board {
  public:
    Board(int tile_n, int initial_meeple_n);
    ~Board();
    TilePositionMap* getTilePositionMap();
    const GameContext* getGameContext() const;
    const std::vector<CityRegion*>* getCityRegions() const;
    const std::vector<CloisterRegion*>* getCloisterRegions() const;
    const std::vector<FieldRegion*>* getFieldRegions() const;
    const std::vector<RoadRegion*>* getRoadRegions() const;
    void registerMeeple(MeepleColor color);
    bool canPlaceTile(Tile* tile, int x, int y, int rotation);
    bool hasPossiblePlacement(Tile* tile);
    void setInitialTile(Tile* tile);
    void setInitialTile(Tile* tile, int rotation);
    bool placeTile(Tile* tile, int x, int y, int rotation,
      std::vector<Segment*>* meeple_place_candidates);
    bool placeMeeple(Segment* segment, MeepleColor color);
    // ゲーム終了後、残りの得点を計算するときに使う
    void transferRemainingPoints(bool return_meeple);
    bool isUndoable() const;
    Tile* undo();
  private:
    bool adjacencyIsValid(Tile* tile, int x, int y, int rotation);
    void undoPlaceTile(TilePlacementEvent* tile_event);
    void undoPlaceMeeple(MeeplePlacementEvent* meeple_event);
    int region_id_;
    TilePositionMap tile_map_;
    GameContext context_;
    std::vector<PlacementEvent*> placement_events_;
    std::vector<CityRegion*> city_regions_;
    std::vector<CloisterRegion*> cloister_regions_;
    std::vector<FieldRegion*> field_regions_;
    std::vector<RoadRegion*> road_regions_;
};

#endif
