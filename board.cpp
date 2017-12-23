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

Board::Board(int tile_n, int initial_meeple_n)
  : region_id_(0), tile_map_(tile_n), context_(initial_meeple_n), placement_events_(),
  city_regions_(), cloister_regions_(), field_regions_(), road_regions_() {
}

Board::~Board() {
  for (auto it = city_regions_.begin(); it != city_regions_.end(); ++it) {
    CityRegion* region = *it;
    delete region;
  }
  for (auto it = cloister_regions_.begin(); it != cloister_regions_.end(); ++it) {
    CloisterRegion* region = *it;
    delete region;
  }
  for (auto it = field_regions_.begin(); it != field_regions_.end(); ++it) {
    FieldRegion* region = *it;
    delete region;
  }
  for (auto it = road_regions_.begin(); it != road_regions_.end(); ++it) {
    RoadRegion* region = *it;
    delete region;
  }
}

TilePositionMap* Board::getTilePositionMap() {
  return &tile_map_;
}

const GameContext* Board::getGameContext() const {
  return &context_;
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

void Board::registerMeeple(MeepleColor color) {
  context_.registerMeeple(color);
}

bool Board::canPlaceTile(Tile* tile, int x, int y, int rotation) const {
  if (!tile_map_.isPlacablePosition(x, y)) {
    return false;
  }
  return adjacencyIsValid(tile, x, y, rotation);
}

bool Board::hasPossiblePlacement(Tile* tile) const {
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

bool Board::adjacencyIsValid(Tile* tile, int x, int y, int rotation) const {
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
  if (placement_events_.size() != 0) {
    return;
  }
  tile->setX(0);
  tile->setY(0);
  tile->setRotation(rotation);
  tile_map_.placeTile(tile, 0, 0);
  TilePlacementEvent* tile_event = new TilePlacementEvent(tile);
  const std::vector<Segment*>* city_segments = tile->getCitySegments();
  for (auto it = city_segments->cbegin(); it != city_segments->cend(); ++it) {
    Segment* s = *it;
    CityRegion* region = new CityRegion(region_id_++, s, this);
    city_regions_.push_back(region);
    tile_event->addGeneratedRegion(region);
  }
  const std::vector<Segment*>* field_segments = tile->getFieldSegments();
  for (auto it = field_segments->cbegin(); it != field_segments->cend(); ++it) {
    Segment* s = *it;
    FieldRegion* region = new FieldRegion(region_id_++, s, this);
    field_regions_.push_back(region);
    tile_event->addGeneratedRegion(region);
  }
  const std::vector<Segment*>* road_segments = tile->getRoadSegments();
  for (auto it = road_segments->cbegin(); it != road_segments->cend(); ++it) {
    Segment* s = *it;
    RoadRegion* region = new RoadRegion(region_id_++, s, this);
    road_regions_.push_back(region);
    tile_event->addGeneratedRegion(region);
  }
  Segment* cloister_segment = tile->getCloisterSegment();
  if (cloister_segment != nullptr) {
    CloisterRegion* region = new CloisterRegion(region_id_++, cloister_segment, this);
    cloister_regions_.push_back(region);
    tile_event->addGeneratedRegion(region);
  }
  PlacementEvent* event = new PlacementEvent(tile_event);
  placement_events_.push_back(event);
}

bool Board::placeTile(Tile* tile, int x, int y, int rotation, std::vector<Segment*>* meeple_place_candidates) {
  assert(meeple_place_candidates->size() == 0);
  if (!canPlaceTile(tile, x, y, rotation)) {
    return false;
  }
  tile->setX(x);
  tile->setY(y);
  tile->setRotation(rotation);
  tile_map_.placeTile(tile, x, y);
  TilePlacementEvent* tile_event = new TilePlacementEvent(tile);

  Tile* around_tiles[4] = {
    tile_map_.getPlacedTile(x, y + 1), tile_map_.getPlacedTile(x + 1, y),
    tile_map_.getPlacedTile(x, y - 1), tile_map_.getPlacedTile(x - 1, y)
  };
  std::vector<Region*> adjacent_regions;

  const std::vector<Segment*>* city_segments = tile->getCitySegments();
  for (auto it = city_segments->cbegin(); it != city_segments->cend(); ++it) {
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
      tile_event->addGeneratedRegion(region);
    } else {
      Region* region = adjacent_regions.at(0);
      region->addSegment(my_s);
      RegionCompositionEvent* composition_event = new RegionCompositionEvent(my_s);
      auto it = adjacent_regions.cbegin();
      it++;
      for (; it != adjacent_regions.cend(); it++) {
        if (region->mergeRegion(*it)) {
	  composition_event->addMergedRegion(*it);
	}
      }
      if (!region->meepleIsPlaced()) {
        meeple_place_candidates->push_back(my_s);
      } else if (region->isCompleted() && !region->pointIsTransfered()) {
	region->transferPoint(&context_, true);
	tile_event->addCompletedRegion(region);
      }
      tile_event->addRegionCompositionEvent(composition_event);
    }
    adjacent_regions.clear();
  }

  const std::vector<Segment*>* road_segments = tile->getRoadSegments();
  for (auto it = road_segments->cbegin(); it != road_segments->cend(); ++it) {
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
      tile_event->addGeneratedRegion(region);
    } else {
      Region* region = adjacent_regions.at(0);
      region->addSegment(my_s);
      RegionCompositionEvent* composition_event = new RegionCompositionEvent(my_s);
      auto it = adjacent_regions.cbegin();
      it++;
      for (; it != adjacent_regions.cend(); it++) {
        if (region->mergeRegion(*it)) {
	  composition_event->addMergedRegion(*it);
	}
      }
      if (!region->meepleIsPlaced()) {
        meeple_place_candidates->push_back(my_s);
      } else if (region->isCompleted() && !region->pointIsTransfered()) {
	region->transferPoint(&context_, true);
	tile_event->addCompletedRegion(region);
      }
      tile_event->addRegionCompositionEvent(composition_event);
    }
    adjacent_regions.clear();
  }

  const std::vector<Segment*>* field_segments = tile->getFieldSegments();
  for (auto it = field_segments->cbegin(); it != field_segments->cend(); ++it) {
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
      tile_event->addGeneratedRegion(region);
    } else {
      Region* region = adjacent_regions.at(0);
      region->addSegment(my_s);
      RegionCompositionEvent* composition_event = new RegionCompositionEvent(my_s);
      auto it = adjacent_regions.cbegin();
      it++;
      for (; it != adjacent_regions.cend(); it++) {
        if (region->mergeRegion(*it)) {
	  composition_event->addMergedRegion(*it);
	}
      }
      if (!region->meepleIsPlaced()) {
        meeple_place_candidates->push_back(my_s);
      }
      tile_event->addRegionCompositionEvent(composition_event);
    }
    adjacent_regions.clear();
  }

  for (auto it = cloister_regions_.begin(); it != cloister_regions_.end(); ++it) {
    CloisterRegion* region = *it;
    if (region->isCompleted() && !region->pointIsTransfered()) {
      region->transferPoint(&context_, true);
      tile_event->addCompletedRegion(region);
    }
  }
  Segment* cloister_segment = tile->getCloisterSegment();
  if (cloister_segment != nullptr) {
    CloisterRegion* region = new CloisterRegion(region_id_++, cloister_segment, this);
    cloister_regions_.push_back(region);
    meeple_place_candidates->push_back(cloister_segment);
    tile_event->addGeneratedRegion(region);
  }

  PlacementEvent* event = new PlacementEvent(tile_event);
  placement_events_.push_back(event);
  return true;
}

bool Board::placeMeeple(Segment* segment, MeepleColor color) {
  Region* region = segment->getRegion();
  if (region->meepleIsPlaced()) {
    return false;
  }
  segment->placeMeeple(color);
  context_.placeMeeple(color);
  if (region->isCompleted()) {
    region->transferPoint(&context_, true);
  }
  MeeplePlacementEvent* meeple_event = new MeeplePlacementEvent(segment);
  PlacementEvent* event = placement_events_.back();
  assert(event->getMeeplePlacementEvent() == nullptr);
  event->setMeeplePlacementEvent(meeple_event);
  return true;
}

void Board::transferRemainingPoints(bool return_meeple) {
  for (auto it = city_regions_.begin(); it != city_regions_.end(); ++it) {
    CityRegion* region = *it;
    if (!region->isMerged() && !region->pointIsTransfered()) {
      region->transferPoint(&context_, return_meeple);
    }
  }
  for (auto it = cloister_regions_.begin(); it != cloister_regions_.end(); ++it) {
    CloisterRegion* region = *it;
    if (!region->isMerged() && !region->pointIsTransfered()) {
      region->transferPoint(&context_, return_meeple);
    }
  }
  for (auto it = field_regions_.begin(); it != field_regions_.end(); ++it) {
    FieldRegion* region = *it;
    if (!region->isMerged() && !region->pointIsTransfered()) {
      region->transferPoint(&context_, return_meeple);
    }
  }
  for (auto it = road_regions_.begin(); it != road_regions_.end(); ++it) {
    RoadRegion* region = *it;
    if (!region->isMerged() && !region->pointIsTransfered()) {
      region->transferPoint(&context_, return_meeple);
    }
  }
}

void Board::endTurn() {
  context_.endTurn();
}

void Board::endGame() {
  context_.endGame();
}

bool Board::isUndoable() const {
  // 最初のタイルはundoできない
  return placement_events_.size() >= 2;
}

Tile* Board::undo() {
  assert(isUndoable());
  PlacementEvent* event = placement_events_.back();
  placement_events_.pop_back();
  MeeplePlacementEvent* meeple_event = event->getMeeplePlacementEvent();
  if (meeple_event != nullptr) {
    undoPlaceMeeple(meeple_event);
  }
  TilePlacementEvent* tile_event = event->getTilePlacementEvent();
  undoPlaceTile(tile_event);
  Tile* tile = tile_event->getTile();
  tile_map_.undo(tile->getX(), tile->getY());
  endTurn();
  tile->setX(0);
  tile->setY(0);
  tile->setRotation(0);
  delete event;
  return tile;
}

void Board::undoPlaceTile(TilePlacementEvent* tile_event) {
  std::vector<Region*>* completed_regions = tile_event->getCompletedRegions();
  for (auto it = completed_regions->begin(); it != completed_regions->end(); ++it) {
    Region* r = *it;
    if (r->pointIsTransfered()) {
      r->undoTransferPoint(&context_, true);
    }
  }
  std::vector<Region*>* generated_regions = tile_event->getGeneratedRegions();
  while (!generated_regions->empty()) {
    Region* generated_region = generated_regions->back();
    generated_regions->pop_back();
    removeRegionFromBoard(generated_region);
    delete generated_region;
  }

  std::vector<RegionCompositionEvent*>* comp_events = tile_event->getRegionCompositionEvents();
  for (auto it = comp_events->begin(); it != comp_events->end(); ++it) {
    RegionCompositionEvent* comp_event = *it;
    Segment* s = comp_event->getAddedSegment();
    Region* base_region = s->getRegion();
    std::vector<Region*>* merged_regions = comp_event->getMergedRegions();
    while (!merged_regions->empty()) {
      Region* merged_region = merged_regions->back();
      merged_regions->pop_back();
      base_region->undoMergeRegion(merged_region);
    }
    base_region->undoAddSegment(s);
  }
}

inline void Board::removeRegionFromBoard(Region* region_to_remove) {
  // TODO: リファクタリング(効率悪い(?)し、汚い)
  switch (region_to_remove->getType()) {
  case RegionType::CITY:
    for (auto it = city_regions_.begin(); it != city_regions_.end(); ++it) {
      Region* region = *it;
      if (region_to_remove == region) {
        city_regions_.erase(it);
        return;
      }
    }
    break;
  case RegionType::CLOISTER:
    for (auto it = cloister_regions_.begin(); it != cloister_regions_.end(); ++it) {
      Region* region = *it;
      if (region_to_remove == region) {
        cloister_regions_.erase(it);
        return;
      }
    }
    break;
  case RegionType::FIELD:
    for (auto it = field_regions_.begin(); it != field_regions_.end(); ++it) {
      Region* region = *it;
      if (region_to_remove == region) {
        field_regions_.erase(it);
        return;
      }
    }
    break;
  case RegionType::ROAD:
    for (auto it = road_regions_.begin(); it != road_regions_.end(); ++it) {
      Region* region = *it;
      if (region_to_remove == region) {
        road_regions_.erase(it);
        return;
      }
    }
    break;
  }
  // ここに到達してはいけない
  assert(false);
}

void Board::undoPlaceMeeple(MeeplePlacementEvent* meeple_event) {
  Segment* s = meeple_event->getSegment();
  Region* r = s->getRegion();
  if (r->isCompleted() && r->pointIsTransfered()) {
    r->undoTransferPoint(&context_, true);
  }
  MeepleColor placed_color = s->undoPlaceMeeple();
  context_.returnMeeple(placed_color, 1);
}
