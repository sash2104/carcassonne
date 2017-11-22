#include <cassert>
#include <vector>

#include "board.hpp"
#include "region.hpp"
#include "segment.hpp"

Region::Region(int id, Board* board) : id_(id), board_(board), merged_(false) {
}

Region::~Region() {
}

int Region::getId() const {
  return id_;
}

Board* Region::getBoard() const {
  return board_;
}

void Region::addSegment(Segment* segment) {
  segments_.push_back(segment);
  segment->setRegion(this);
}

const std::vector<Segment*>* Region::getSegments() const {
  return &segments_;
}

bool Region::mergeRegion(Region* region) {
  if (this != region && !region->isMerged()) {
    auto segments = region->getSegments();
    for (auto it = segments->cbegin(); it != segments->cend(); it++) {
      Segment* s = *it;
      addSegment(s);
      if (s->meepleIsPlaced()) {
	meeple_placed_segments_.push_back(s);
      }
    }
    region->merged();
    return true;
  }
  return false;
}

bool Region::isMerged() const {
  return merged_;
}

void Region::merged() {
  merged_ = true;
}

void Region::meepleIsPlacedOnSegment(Segment* segment) {
  assert(segment->getRegion() == this);
  assert(segment->meepleIsPlaced());
  meeple_placed_segments_.push_back(segment);
}

const std::vector<Segment*>* Region::getMeeplePlacedSegments() const {
  return &meeple_placed_segments_;
}

bool Region::meepleIsPlaced() const {
  return meeple_placed_segments_.size() != 0;
}

CityRegion::CityRegion(int id, Board* board) : Region(id, board) {
}

bool CityRegion::isCompleted() {
  // TODO
  return false;
}

int CityRegion::calculatePoint() {
  // TODO
  return 0;
}

inline RegionType CityRegion::getType() const {
  return RegionType::CITY;
}


CloisterRegion::CloisterRegion(int id, Board* board) : Region(id, board), completed_(false) {
}

bool CloisterRegion::isCompleted() {
  if (completed_) {
    return true;
  }
  return calculatePoint() == 9;
}

int CloisterRegion::calculatePoint() {
  if (completed_) {
    return 9;
  }
  const TilePositionMap* m = getBoard()->getTilePositionMap();
  const std::vector<Segment*>* segments = getSegments();
  assert(segments->size() == 1);
  const Segment* segment = segments->at(0);
  const Tile* tile = segment->getTile();
  int x = tile->getX();
  int y = tile->getY();
  int point = 0;
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (m->isTilePlaced(x + dx, y + dy)) {
	point++;
      }
    }
  }
  if (point == 9) {
    completed_ = true;
  }
  return point;
}

inline RegionType CloisterRegion::getType() const {
  return RegionType::CLOISTER;
}


FieldRegion::FieldRegion(int id, Board* board) : Region(id, board) {
}

bool FieldRegion::isCompleted() {
  // TODO
  return false;
}

int FieldRegion::calculatePoint() {
  // TODO
  return 0;
}

inline RegionType FieldRegion::getType() const {
  return RegionType::FIELD;
}


RoadRegion::RoadRegion(int id, Board* board) : Region(id, board) {
}

bool RoadRegion::isCompleted() {
  // TODO
  return false;
}

int RoadRegion::calculatePoint() {
  // TODO
  return 0;
}

inline RegionType RoadRegion::getType() const {
  return RegionType::ROAD;
}
