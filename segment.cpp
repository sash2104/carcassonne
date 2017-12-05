#include <cassert>

#include "region.hpp"
#include "segment.hpp"
#include "tile.hpp"
#include "utils.hpp"

Segment::Segment(int index, SegmentType type, bool has_pennant) :
  index_(index), type_(type), has_pennant_(has_pennant), placed_meeple_(MeepleColor::NOT_PLACED) {
}

int Segment::getIndex() const {
  return index_;
}

SegmentType Segment::getType() const {
  return type_;
}

Tile* Segment::getTile() const {
  return tile_;
}

void Segment::setTile(Tile* tile) {
  tile_ = tile;
}

Region* Segment::getRegion() const {
  return region_;
}

void Segment::setRegion(Region* region) {
  region_ = region;
}

bool Segment::hasPennant() const {
  return has_pennant_;
}

bool Segment::isAdjacentTo(int direction) const {
  assert(direction >= 0 && direction < 4);
  int pre_rotated_d;
  if (type_ != SegmentType::FIELD) {
    pre_rotated_d = modBy4(direction - tile_->getRotation());
  } else {
    pre_rotated_d = modBy8(direction - tile_->getRotation() * 2);
  }
  switch (type_) {
  case SegmentType::CITY:
    return tile_->getCities()[pre_rotated_d] == index_;
  case SegmentType::ROAD:
    return tile_->getRoads()[pre_rotated_d] == index_;
  case SegmentType::FIELD:
    return tile_->getFields()[pre_rotated_d] == index_;
  case SegmentType::CLOISTER:
    return false;
  }
  // ここには到達しない
  assert(false);
  return true;
}

void Segment::placeMeeple(MeepleColor meeple) {
  assert(meeple != MeepleColor::NOT_PLACED);
  placed_meeple_ = meeple;
}

MeepleColor Segment::getPlacedMeeple() const {
  assert(placed_meeple_ != MeepleColor::NOT_PLACED);
  return placed_meeple_;
}

bool Segment::meepleIsPlaced() const {
  return placed_meeple_ != MeepleColor::NOT_PLACED;
}
