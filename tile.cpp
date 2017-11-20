#include <cassert>
#include <iostream>
#include <vector>

#include "segment.hpp"
#include "tile.hpp"

// 2進数で表示
void print_bit_tile(int bit_tile) {
  int bit_offset = 2048;
  while (bit_offset > 0) {
    if (bit_tile & bit_offset) { std::cerr << 1; }
    else { std::cerr << 0; }
    bit_offset /= 2;
  }
  std::cerr << std::endl;
}

Tile::Tile(int id, char* name, BorderType* border_types, int* cities, int* roads, int* fields)
  : id_(id), name_(name) {
  for (int i = 0; i < 4; i++) {
    border_types_[i] = border_types[i];
    cities_[i] = cities[i];
    roads_[i] = roads[i];
    fields_[2*i] = fields[2*i];
    fields_[2*i+1] = fields[2*i+1];
  }
}

Tile::Tile(int id) : id_(id) {
}

Tile::~Tile() {
}

int Tile::getId() const { return id_; }

const char* Tile::getName() const { return name_; }

int Tile::getX() const { return x_; }

int Tile::getY() const { return y_; }

int Tile::getRotation() const { return rotation_; }

void Tile::setX(int x) { x_ = x; }

void Tile::setY(int y) { y_ = y; }

void Tile::setRotation(int rotation) { rotation_ = rotation; }

BorderType Tile::getBorderType(int direction) {
  assert(direction >= 0 && direction < 4);
  return border_types_[direction];
}

const int* Tile::getCities() const {
  return cities_;
}

const int* Tile::getRoads() const {
  return roads_;
}

const int* Tile::getFields() const {
  return fields_;
}

const Segment* Tile::getCitySegmentOfDirection(int direction) const {
  // TODO
  return nullptr;
}

const Segment* Tile::getFieldSegmentOfDirection(int direction) const {
  // TODO
  return nullptr;
}

const Segment* Tile::getRoadSegmentOfDirection(int direction) const {
  // TODO
  return nullptr;
}

const std::vector<Segment*>* Tile::getCitySegments() const {
  // TODO
  return nullptr;
}

const std::vector<Segment*>* Tile::getFieldSegments() const {
  // TODO
  return nullptr;
}

const std::vector<Segment*>* Tile::getRoadSegments() const {
  // TODO
  return nullptr;
}

Segment* Tile::getCloisterSegment() {
  // TODO
  return nullptr;
}
