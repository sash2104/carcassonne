#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "board.hpp"
#include "game_context.hpp"
#include "region.hpp"
#include "segment.hpp"
#include "utils.hpp"

SegmentIterator::SegmentIterator(const SegmentIterator& iter)
  : root_(iter.root_), current_(iter.current_), iter_(iter.iter_) {
}

SegmentIterator::SegmentIterator(const Region* root, const Region* current,
  std::vector<Segment*>::const_iterator iter)
  : root_(root), current_(root), iter_(iter) {
}

SegmentIterator& SegmentIterator::operator++() {
  iter_++;
  advanceToActualNext();
  return *this;
}

Segment* SegmentIterator::operator*() {
  return *iter_;
}

bool SegmentIterator::operator==(const SegmentIterator& iter) {
  return root_ == iter.root_ && current_ == iter.current_ && iter_ == iter.iter_;
}

bool SegmentIterator::operator!=(const SegmentIterator& iter) {
  return root_ != iter.root_ || current_ != iter.current_ || iter_ != iter.iter_;
}

inline void SegmentIterator::advanceToActualNext() {
  if (iter_ != current_->segments_.end()) {
    return;
  }
  if (current_->last_child_ != nullptr) {
    current_ = current_->last_child_;
    iter_ = current_->segments_.begin();
    return;
   }
  for (const Region* region = current_; region != root_; region = region->parent_) {
    if (region->prev_sibling_ != nullptr) {
      current_ = region->prev_sibling_;
      iter_ = current_->segments_.begin();
      return;
    }
  }
  setToEnd();
}

inline void SegmentIterator::setToEnd() {
  current_ = root_;
  iter_ = current_->segments_.end();
}


Region::Region(int id, Segment* segment, Board* board)
  : id_(id), board_(board), segments_(), meeple_placed_count_(0), winning_meeples_(),
    parent_(nullptr), first_child_(nullptr), last_child_(nullptr), prev_sibling_(nullptr),
    point_transfered_(false) {
  addSegment(segment);
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
  assert(segment->getRegion() == nullptr);
  segments_.push_back(segment);
  segment->setRegion(this);
}

void Region::undoAddSegment(Segment* segment) {
  assert(segment->getRegion() == this);
  assert(!segment->meepleIsPlaced()); // ミープルは取り除いておく必要がある
  assert(segments_.size() > 1); // 最初のセグメントは取り除けない
  for (auto it = segments_.begin(); it != segments_.end(); ++it) {
    Segment* s = *it;
    if (s == segment) {
      segments_.erase(it);
      segment->unsetRegion();
      rewindRegionState();
      return;
    }
  }
  // ここに来てはいけない
  assert(false);
}

SegmentIterator Region::segmentBegin() const {
  return SegmentIterator(this, this, segments_.begin());
}

SegmentIterator Region::segmentEnd() const {
  return SegmentIterator(this, this, segments_.end());
}

bool Region::mergeRegion(Region* region) {
  assert(getType() == region->getType());
  if (this != region && !region->isMerged()) {
    for (auto it = region->segmentBegin(); it != region->segmentEnd(); ++it) {
      Segment* s = *it;
      s->setRegion(this);
    }
    appendChild(region);
    meeple_placed_count_ += region->meeple_placed_count_;
    return true;
  }
  return false;
}

inline void Region::appendChild(Region* region) {
  if (first_child_ == nullptr) {
    first_child_ = last_child_ = region;
  } else {
    region->prev_sibling_ = last_child_;
    last_child_ = region;
  }
  region->parent_ = this;
}

void Region::undoMergeRegion(Region* region) {
  assert(region->parent_ == this);
  assert(last_child_ == region);
  for (auto it = region->segmentBegin(); it != region->segmentEnd(); ++it) {
    Segment* s = *it;
    s->setRegion(region);
  }
  removeLastChild();
  meeple_placed_count_ -= region->meeple_placed_count_;
  rewindRegionState();
}

inline void Region::removeLastChild() {
  Region* removed = last_child_;
  last_child_ = removed->prev_sibling_;
  if (last_child_ == nullptr) {
    first_child_ = nullptr;
  } else {
    removed->prev_sibling_ = nullptr;
  }
  removed->parent_ = nullptr;
}

bool Region::isMerged() const {
  return parent_ != nullptr;
}

void Region::meepleIsPlacedOnSegment(Segment* segment) {
  assert(segment->getRegion() == this);
  assert(segment->meepleIsPlaced());
  // TODO: 色毎にカウントする
  ++meeple_placed_count_;
}

void Region::meepleIsUnplacedOnSegment(Segment* segment) {
  assert(segment->getRegion() == this);
  assert(!segment->meepleIsPlaced());
  --meeple_placed_count_;
}

bool Region::meepleIsPlaced() const {
  return meeple_placed_count_ != 0;
}

void Region::getWinningMeeples(std::vector<MeepleColor>* winning_meeples) const {
  assert(winning_meeples->size() == 0);
  std::map<int, int> counts;
  for (auto it = segmentBegin(); it != segmentEnd(); ++it) {
    Segment* s = *it;
    if (!s->meepleIsPlaced()) {
      continue;
    }
    MeepleColor color = s->getPlacedMeeple();
    assert(color != MeepleColor::NOT_PLACED);
    int k = static_cast<int>(color);
    int count;
    auto it2 = counts.find(k);
    if (counts.find(k) == counts.end()) {
      count = 1;
    } else {
      count = it2->second + 1;
    }
    counts[k] = count;
  }
  int max_count = 1;
  for (auto it = counts.begin(); it != counts.end(); ++it) {
    int count = it->second;
    if (count > max_count) {
      max_count = count;
    }
  }
  for (auto it = counts.begin(); it != counts.end(); ++it) {
    int k = it->first;
    int count = it->second;
    if (count == max_count) {
      winning_meeples->push_back(static_cast<MeepleColor>(k));
    }
  }
}

void Region::returnMeeples(GameContext* context) {
  for (auto it = segmentBegin(); it != segmentEnd(); ++it) {
    Segment* s = *it;
    if (s->meepleIsPlaced()) {
      MeepleColor color = s->getPlacedMeeple();
      context->returnMeeple(color, 1);
    }
  }
}

void Region::undoReturnMeeples(GameContext* context) {
  for (auto it = segmentBegin(); it != segmentEnd(); ++it) {
    Segment* s = *it;
    if (s->meepleIsPlaced()) {
      MeepleColor color = s->getPlacedMeeple();
      context->placeMeeple(color);
    }
  }
}

void Region::transferPoint(GameContext* context, bool return_meeple) {
  int point = calculatePoint();
  std::vector<MeepleColor> winning_meeples;
  getWinningMeeples(&winning_meeples);
  for (auto it = winning_meeples.begin(); it != winning_meeples.end(); ++it) {
    MeepleColor color = *it;
    context->addPoint(color, getType(), point);
  }
  point_transfered_ = true;
  if (return_meeple) {
    returnMeeples(context);
  }
}

void Region::undoTransferPoint(GameContext* context, bool undo_return_meeple) {
  int point = -calculatePoint();
  std::vector<MeepleColor> winning_meeples;
  getWinningMeeples(&winning_meeples);
  for (auto it = winning_meeples.begin(); it != winning_meeples.end(); ++it) {
    MeepleColor color = *it;
    context->addPoint(color, getType(), point);
  }
  point_transfered_ = false;
  if (undo_return_meeple) {
    undoReturnMeeples(context);
  }
}

bool Region::pointIsTransfered() const {
  return point_transfered_;
}

void Region::debugPrint() const {
  std::cout << "Region#" << id_ << "(" << isMerged() << "): ";
  for (auto it = segments_.begin(); it != segments_.end(); ++it) {
    Segment* s = *it;
    std::cout << s->getIndex() << "th ";
    std::cout << static_cast<int>(s->getType()) << " Segment of " << "Tile#" << s->getTile()->getId() << ", ";
  }
  std::cout << std::endl;
}


CityRegion::CityRegion(int id, Segment* segment, Board* board) : Region(id, segment, board), completed_(false) {
}

bool CityRegion::isCompleted() {
  if (completed_) {
    return true;
  }
  for (auto it = segmentBegin(); it != segmentEnd(); ++it) {
    Segment* s = *it;
    for (int d = 0; d < 4; d++) {
      if (!s->isAdjacentTo(d)) {
        continue;
      }
      std::pair<int, int> p = adjacentCoordinate(s->getTile()->getX(), s->getTile()->getY(), d);
      int adjacent_x = p.first;
      int adjacent_y = p.second;
      bool adjacent_segment_is_found = false;
      for (auto it2 = segmentBegin(); it2 != segmentEnd(); ++it2) {
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
  int pennant_count = 0;
  std::set<int> uniq_tiles;
  for (auto it = segmentBegin(); it != segmentEnd(); ++it) {
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

void CityRegion::rewindRegionState() {
  completed_ = false;
}


CloisterRegion::CloisterRegion(int id, Segment* segment, Board* board) : Region(id, segment, board), completed_(false) {
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
  assert(segments_.size() == 1);
  const Segment* segment = segments_.at(0);
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

void CloisterRegion::rewindRegionState() {
  completed_ = false;
}


FieldRegion::FieldRegion(int id, Segment* segment, Board* board) : Region(id, segment, board) {
}

bool FieldRegion::isCompleted() {
  return false;
}

int FieldRegion::calculatePoint() {
 const std::vector<CityRegion*>* city_regions = getBoard()->getCityRegions();
 return calculatePoint(city_regions);
}

int FieldRegion::calculatePoint(const std::vector<CityRegion*>* city_regions) {
  int point = 0;
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
  for (auto field_it = segmentBegin(); field_it != segmentEnd(); ++field_it) {
    Segment* field_s = *field_it;
    Tile* field_t = field_s->getTile();
    for (auto city_it = city_region->segmentBegin(); city_it != city_region->segmentEnd(); ++city_it) {
      Segment* city_s = *city_it;
      Tile* city_t = city_s->getTile();
      if (field_t->getX() == city_t->getX() && field_t->getY() == city_t->getY()) {
        assert(field_t == city_t);
        if (field_t->isTwoSegmentAdjacent(field_s->getIndex(), city_s->getIndex())) {
          return true;
        }
      }
    }
  }
  return false;
}

void FieldRegion::rewindRegionState() {
}


RoadRegion::RoadRegion(int id, Segment* segment, Board* board) : Region(id, segment, board), completed_(false) {
}

bool RoadRegion::isCompleted() {
  if (completed_) {
    return true;
  }
  for (auto it = segmentBegin(); it != segmentEnd(); ++it) {
    Segment* s = *it;
    for (int d = 0; d < 4; d++) {
      if (!s->isAdjacentTo(d)) {
        continue;
      }
      std::pair<int, int> p = adjacentCoordinate(s->getTile()->getX(), s->getTile()->getY(), d);
      int adjacent_x = p.first;
      int adjacent_y = p.second;
      bool adjacent_segment_is_found = false;
      for (auto it2 = segmentBegin(); it2 != segmentEnd(); ++it2) {
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
  std::set<int> uniq_tiles;
  for (auto it = segmentBegin(); it != segmentEnd(); ++it) {
    Segment* s = *it;
    uniq_tiles.insert(s->getTile()->getId());
  }
  return uniq_tiles.size();
}

inline RegionType RoadRegion::getType() const {
  return RegionType::ROAD;
}

void RoadRegion::rewindRegionState() {  
  completed_ = false;
}
