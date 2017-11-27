#include <cassert>
#include <set>
#include <utility>
#include <vector>

#include "board.hpp"
#include "region.hpp"
#include "segment.hpp"
#include "utils.hpp"

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
  if (completed_) {
    return true;
  }
  const std::vector<Segment*>* segments = getSegments();
  for (auto it = segments->cbegin(); it != segments->cend(); it++) {
    Segment* s = *it;
    for (int d = 0; d < 4; d++) {
      if (!s->isAdjacentTo(d)) {
        continue;
      }
      std::pair<int, int> p = adjacentCoordinate(s->getTile()->getX(), s->getTile()->getY(), d);
      int adjacent_x = p.first;
      int adjacent_y = p.second;
      bool adjacent_segment_is_found = false;
      for (auto it2 = segments->cbegin(); it2 != segments->cend(); it2++) {
        Segment* s2 = *it2;
        if (s2->getTile()->getX() == adjacent_x && s2->getTile()->getY() == adjacent_y &&
            s2->isAdjacentTo(modBy4(d + 2))) {
          adjacent_segment_is_found = true;
          break;
        }
      }
      if (!adjacent_segment_is_found) {
        return false;
      }
    }
  }
  completed_ = true;
  return true;
}

int CityRegion::calculatePoint() {
  const std::vector<Segment*>* segments = getSegments();
  int pennant_count = 0;
  std::set<int> uniq_tiles;
  for (auto it = segments->cbegin(); it != segments->cend(); it++) {
    Segment* s = *it;
    std::pair<std::set<int>::iterator, bool> res = uniq_tiles.insert(s->getTile()->getId());
    if (res.second) {
      if (s->hasPennant()) {
        pennant_count++;
      }
    }
  }
  int point = pennant_count + uniq_tiles.size();
  return isCompleted() ? point * 2 : point;
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
  return false;
}

int FieldRegion::calculatePoint() {
  int point = 0;
 const  std::vector<CityRegion*>* city_regions = getBoard()->getCityRegions();
  for (auto it = city_regions->cbegin(); it != city_regions->cend(); it++) {
    CityRegion* region = *it;
    if (!region->isMerged() && region->isCompleted() && isAdjacentWith(region)) {
      point += 3;
    }
  }
  return point;
}

inline RegionType FieldRegion::getType() const {
  return RegionType::FIELD;
}

bool FieldRegion::isAdjacentWith(const CityRegion* city_region) {
  const std::vector<Segment*>* field_segments = getSegments();
  const std::vector<Segment*>* city_segments = city_region->getSegments();
  for (auto field_it = field_segments->cbegin(); field_it != field_segments->cend(); field_it++) {
    Segment* field_s = *field_it;
    Tile* field_t = field_s->getTile();
    for (auto city_it = city_segments->cbegin(); city_it != city_segments->cend(); city_it++) {
      Segment* city_s = *city_it;
      Tile* city_t = city_s->getTile();
      if (field_t->getX() == city_t->getX() && field_t->getY() == city_t->getY()) {
        if (field_t->isTwoSegmentAdjacent(field_s->getIndex(), city_s->getIndex())) {
          return true;
        }
      }
    }
  }
  return false;
}

RoadRegion::RoadRegion(int id, Board* board) : Region(id, board) {
}

bool RoadRegion::isCompleted() {
  if (completed_) {
    return true;
  }
  const std::vector<Segment*>* segments = getSegments();
  for (auto it = segments->cbegin(); it != segments->cend(); it++) {
    Segment* s = *it;
    for (int d = 0; d < 4; d++) {
      if (!s->isAdjacentTo(d)) {
        continue;
      }
      std::pair<int, int> p = adjacentCoordinate(s->getTile()->getX(), s->getTile()->getY(), d);
      int adjacent_x = p.first;
      int adjacent_y = p.second;
      bool adjacent_segment_is_found = false;
      for (auto it2 = segments->cbegin(); it2 != segments->cend(); it2++) {
        Segment* s2 = *it2;
        if (s2->getTile()->getX() == adjacent_x && s2->getTile()->getY() == adjacent_y &&
            s2->isAdjacentTo(modBy4(d + 2))) {
          adjacent_segment_is_found = true;
          break;
        }
      }
      if (!adjacent_segment_is_found) {
        return false;
      }
    }
  }
  completed_ = true;
  return true;
}

int RoadRegion::calculatePoint() {
  const std::vector<Segment*>* segments = getSegments();
  std::set<int> uniq_tiles;
  for (auto it = segments->cbegin(); it != segments->cend(); it++) {
    Segment* s = *it;
    uniq_tiles.insert(s->getTile()->getId());
  }
  return uniq_tiles.size();
}

inline RegionType RoadRegion::getType() const {
  return RegionType::ROAD;
}
