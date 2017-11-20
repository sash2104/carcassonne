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
      s->setRegion(this);
      addSegment(s);
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

bool Region::meepleIsPlaced() const {
  // TODO
  return false;
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
  /*
  const TilePositionMap* m = getBoard()->getTilePositionMap();
  const std::vector<Segment*>* segments = getSegments();
  assert(segments->size() == 1);
  const Segment* segment = segments->at(0);
*/
  return 0;
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
