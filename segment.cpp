
#include "region.hpp"
#include "segment.hpp"
#include "tile.hpp"

Segment::Segment(int index, SegmentType type, bool has_pennant) :
  index_(index), type_(type), has_pennant_(has_pennant) {
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
  // TODO
  return true;
}

void Segment::placeMeeple(int meepleColor) {
  // TODO
}
