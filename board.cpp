#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

#include "board.hpp"
#include "game_context.hpp"
#include "meeple_color.hpp"
#include "region.hpp"
#include "segment.hpp"
#include "tile.hpp"
#include "utils.hpp"

Board::Board(int tile_n) : region_id_(0), tile_map_(tile_n), context_(7), placed_tiles_(), city_regions_(), cloister_regions_(), field_regions_(), road_regions_() {
}

Board::~Board() {
  for (auto it = city_regions_.begin(); it != city_regions_.end(); it++) {
    CityRegion* region = *it;
    delete region;
  }
  for (auto it = cloister_regions_.begin(); it != cloister_regions_.end(); it++) {
    CloisterRegion* region = *it;
    delete region;
  }
  for (auto it = field_regions_.begin(); it != field_regions_.end(); it++) {
    FieldRegion* region = *it;
    delete region;
  }
  for (auto it = road_regions_.begin(); it != road_regions_.end(); it++) {
    RoadRegion* region = *it;
    delete region;
  }
}

TilePositionMap* Board::getTilePositionMap() {
  return &tile_map_;
}

const std::vector<CityRegion*>* Board::getCityRegions() const {
  return &city_regions_;
}

const std::vector<CloisterRegion*>* Board::getCloisterRegions() const {
  return &cloister_regions_;
}

const std::vector<FieldRegion*>* Board::getFieldRegions() const {
  return &field_regions_;
}

const std::vector<RoadRegion*>* Board::getRoadRegions() const {
  return &road_regions_;
}

bool Board::canPlaceTile(Tile* tile, int x, int y, int rotation) {
  if (!tile_map_.isPlacablePosition(x, y)) {
    return false;
  }
  return adjacencyIsValid(tile, x, y, rotation);
}

bool Board::hasPossiblePlacement(Tile* tile) {
  const std::set<BoardPosition>* positions = tile_map_.getPlacablePositions();
  for (auto it = positions->cbegin(); it != positions->cend(); it++) {
    BoardPosition pos(*it);
    for (int rotation = 0; rotation < 4; rotation++) {
      if (adjacencyIsValid(tile, pos.getX(), pos.getY(), rotation)) {
        return true;
      }
    }
  }
  return false;
}

bool Board::adjacencyIsValid(Tile* tile, int x, int y, int rotation) {
  Tile* top_tile = tile_map_.getPlacedTile(x, y + 1);
  if (top_tile != nullptr && !top_tile->canBottomAdjacentWith(tile, rotation)) {
    return false;
  }
  Tile* right_tile = tile_map_.getPlacedTile(x + 1, y);
  if (right_tile != nullptr && !right_tile->canLeftAdjacentWith(tile, rotation)) {
    return false;
  }
  Tile* bottom_tile = tile_map_.getPlacedTile(x, y - 1);
  if (bottom_tile != nullptr && !bottom_tile->canTopAdjacentWith(tile, rotation)) {
    return false;
  }
  Tile* left_tile = tile_map_.getPlacedTile(x - 1, y);
  if (left_tile != nullptr && !left_tile->canRightAdjacentWith(tile, rotation)) {
    return false;
  }
  return true;
}

void Board::setInitialTile(Tile* tile) {
  setInitialTile(tile, 0);
}

void Board::setInitialTile(Tile* tile, int rotation) {
  if (placed_tiles_.size() != 0) {
    return;
  }
  tile->setX(0);
  tile->setY(0);
  tile->setRotation(rotation);
  tile_map_.placeTile(tile, 0, 0);
  placed_tiles_.push_back(tile);
  const std::vector<Segment*>* city_segments = tile->getCitySegments();
  for (auto it = city_segments->cbegin(); it != city_segments->cend(); it++) {
    Segment* s = *it;
    CityRegion* region = new CityRegion(region_id_++, s, this);
    city_regions_.push_back(region);
  }
  const std::vector<Segment*>* field_segments = tile->getFieldSegments();
  for (auto it = field_segments->cbegin(); it != field_segments->cend(); it++) {
    Segment* s = *it;
    FieldRegion* region = new FieldRegion(region_id_++, s, this);
    field_regions_.push_back(region);
  }
  const std::vector<Segment*>* road_segments = tile->getRoadSegments();
  for (auto it = road_segments->cbegin(); it != road_segments->cend(); it++) {
    Segment* s = *it;
    RoadRegion* region = new RoadRegion(region_id_++, s, this);
    road_regions_.push_back(region);
  }
  Segment* cloister_segment = tile->getCloisterSegment();
  if (cloister_segment != nullptr) {
    CloisterRegion* region = new CloisterRegion(region_id_++, cloister_segment, this);
    cloister_regions_.push_back(region);
  }
}

bool Board::placeTile(Tile* tile, int x, int y, int rotation, std::vector<Segment*>* meeple_place_candidates, GameContext* context) {
  assert(meeple_place_candidates->size() == 0);
  if (!canPlaceTile(tile, x, y, rotation)) {
    return false;
  }
  tile->setX(x);
  tile->setY(y);
  tile->setRotation(rotation);
  tile_map_.placeTile(tile, x, y);
  placed_tiles_.push_back(tile);

  Tile* around_tiles[4] = {
    tile_map_.getPlacedTile(x, y + 1), tile_map_.getPlacedTile(x + 1, y),
    tile_map_.getPlacedTile(x, y - 1), tile_map_.getPlacedTile(x - 1, y)
  };
  std::vector<Region*> adjacent_regions;

  const std::vector<Segment*>* city_segments = tile->getCitySegments();
  for (auto it = city_segments->cbegin(); it != city_segments->cend(); it++) {
    Segment* my_s = *it;
    for (int d = 0; d < 4; d++) {
      if (around_tiles[d] == nullptr) {
        continue;
      }
      Tile* around_tile = around_tiles[d];
      if (my_s->isAdjacentTo(d)) {
        Segment* your_s = around_tile->getCitySegmentOfDirection(modBy4(d + 2));
        adjacent_regions.push_back(your_s->getRegion());
      }
    }
    if (adjacent_regions.size() == 0) {
      CityRegion* region = new CityRegion(region_id_++, my_s, this);
      city_regions_.push_back(region);
      meeple_place_candidates->push_back(my_s);
    } else {
      Region* region = adjacent_regions.at(0);
      region->addSegment(my_s);
      auto it = adjacent_regions.cbegin();
      it++;
      for (; it != adjacent_regions.cend(); it++) {
        region->mergeRegion(*it);
      }
      if (!region->meepleIsPlaced()) {
        meeple_place_candidates->push_back(my_s);
      } else if (region->isCompleted() && !region->pointIsTransfered()) {
	region->transferPoint(context, true);
      }
    }
    adjacent_regions.clear();
  }

  const std::vector<Segment*>* road_segments = tile->getRoadSegments();
  for (auto it = road_segments->cbegin(); it != road_segments->cend(); it++) {
    Segment* my_s = *it;
    for (int d = 0; d < 4; d++) {
      if (around_tiles[d] == nullptr) {
        continue;
      }
      Tile* around_tile = around_tiles[d];
      if (my_s->isAdjacentTo(d)) {
        Segment* your_s = around_tile->getRoadSegmentOfDirection(modBy4(d + 2));
        adjacent_regions.push_back(your_s->getRegion());
      }
    }
    if (adjacent_regions.size() == 0) {
      RoadRegion* region = new RoadRegion(region_id_++, my_s, this);
      road_regions_.push_back(region);
      meeple_place_candidates->push_back(my_s);
    } else {
      Region* region = adjacent_regions.at(0);
      region->addSegment(my_s);
      auto it = adjacent_regions.cbegin();
      it++;
      for (; it != adjacent_regions.cend(); it++) {
        region->mergeRegion(*it);
      }
      if (!region->meepleIsPlaced()) {
        meeple_place_candidates->push_back(my_s);
      } else if (region->isCompleted() && !region->pointIsTransfered()) {
	region->transferPoint(context, true);
      }
    }
    adjacent_regions.clear();
  }

  const std::vector<Segment*>* field_segments = tile->getFieldSegments();
  for (auto it = field_segments->cbegin(); it != field_segments->cend(); it++) {
    Segment* my_s = *it;
    for (int d = 0; d < 8; d++) {
      if (around_tiles[d / 2] == nullptr) {
	d++;
        continue;
      }
      Tile* around_tile = around_tiles[d / 2];
      if (my_s->isAdjacentTo(d)) {
        Segment* your_s = around_tile->getFieldSegmentOfDirection(modBy8(d + (d % 2 == 0 ? 5 : 3)));
        adjacent_regions.push_back(your_s->getRegion());
      }
    }
    if (adjacent_regions.size() == 0) {
      FieldRegion* region = new FieldRegion(region_id_++, my_s, this);
      field_regions_.push_back(region);
      meeple_place_candidates->push_back(my_s);
    } else {
      Region* region = adjacent_regions.at(0);
      region->addSegment(my_s);
      auto it = adjacent_regions.cbegin();
      it++;
      for (; it != adjacent_regions.cend(); it++) {
        region->mergeRegion(*it);
      }
      if (!region->meepleIsPlaced()) {
        meeple_place_candidates->push_back(my_s);
      }
    }
    adjacent_regions.clear();
  }

  for (auto it = cloister_regions_.begin(); it != cloister_regions_.end(); it++) {
    CloisterRegion* region = *it;
    if (region->isCompleted() && !region->pointIsTransfered()) {
      region->transferPoint(context, true);
    }
  }
  Segment* cloister_segment = tile->getCloisterSegment();
  if (cloister_segment != nullptr) {
    CloisterRegion* region = new CloisterRegion(region_id_++, cloister_segment, this);
    cloister_regions_.push_back(region);
    meeple_place_candidates->push_back(cloister_segment);
  }

  return true;
}

bool Board::placeMeeple(Segment* segment, MeepleColor color, GameContext* context) {
  Region* region = segment->getRegion();
  if (region->meepleIsPlaced()) {
    return false;
  }
  segment->placeMeeple(color);
  context->placeMeeple(color);
  if (region->isCompleted()) {
    region->transferPoint(context, true);
  }
  return true;
}

void Board::transferRemainingPoints(GameContext* context, bool return_meeple) {
  for (auto it = city_regions_.begin(); it != city_regions_.end(); it++) {
    CityRegion* region = *it;
    if (!region->isMerged() && !region->pointIsTransfered()) {
      region->transferPoint(context, return_meeple);
    }
  }
  for (auto it = cloister_regions_.begin(); it != cloister_regions_.end(); it++) {
    CloisterRegion* region = *it;
    if (!region->isMerged() && !region->pointIsTransfered()) {
      region->transferPoint(context, return_meeple);
    }
  }
  for (auto it = field_regions_.begin(); it != field_regions_.end(); it++) {
    FieldRegion* region = *it;
    if (!region->isMerged() && !region->pointIsTransfered()) {
      region->transferPoint(context, return_meeple);
    }
  }
  for (auto it = road_regions_.begin(); it != road_regions_.end(); it++) {
    RoadRegion* region = *it;
    if (!region->isMerged() && !region->pointIsTransfered()) {
      region->transferPoint(context, return_meeple);
    }
  }
}
