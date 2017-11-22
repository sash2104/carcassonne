#include <cassert>
#include <iostream>
#include <vector>

#include "segment.hpp"
#include "tile.hpp"

Tile::Tile(int id, char* name, BorderType* border_types, int* cities, int* roads, int* fields,
  std::vector<Segment*>* city_segments, std::vector<Segment*>* field_segments,
  std::vector<Segment*>* road_segments, Segment* cloister_segment)
  : id_(id), name_(name), city_segments_(city_segments),
    field_segments_(field_segments), cloister_segment_(cloister_segment) {
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
  for (auto iter = city_segments_->begin(); iter != city_segments_->end(); iter++) {
    Segment* s = *iter;
    delete s;
  }
  delete city_segments_;
  for (auto iter = road_segments_->begin(); iter != road_segments_->end(); iter++) {
    Segment* s = *iter;
    delete s;
  }
  delete road_segments_;
  for (auto iter = field_segments_->begin(); iter != field_segments_->end(); iter++) {
    Segment* s = *iter;
    delete s;
  }
  delete field_segments_;
  if (cloister_segment_ != nullptr) {
    delete cloister_segment_;
  }
}

int Tile::getId() const { return id_; }

const char* Tile::getName() const { return name_; }

int Tile::getX() const { return x_; }

int Tile::getY() const { return y_; }

int Tile::getRotation() const { return rotation_; }

void Tile::setX(int x) { x_ = x; }

void Tile::setY(int y) { y_ = y; }

void Tile::setRotation(int rotation) {
  assert(rotation >= 0 && rotation < 4);
  rotation_ = rotation;
}

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
  assert(direction >= 0 && direction < 4);
  int index = cities_[direction];
  return (index == -1) ? nullptr : city_segments_->at(index);
}

const Segment* Tile::getFieldSegmentOfDirection(int direction) const {
  assert(direction >= 0 && direction < 8);
  int index = fields_[direction];
  return (index == -1) ? nullptr : field_segments_->at(index);
}

const Segment* Tile::getRoadSegmentOfDirection(int direction) const {
  assert(direction >= 0 && direction < 4);
  int index = roads_[direction];
  return (index == -1) ? nullptr : road_segments_->at(index);
}

const std::vector<Segment*>* Tile::getCitySegments() const {
  return city_segments_;
}

const std::vector<Segment*>* Tile::getFieldSegments() const {
  return field_segments_;
}

const std::vector<Segment*>* Tile::getRoadSegments() const {
  return road_segments_;
}

Segment* Tile::getCloisterSegment() {
  return cloister_segment_;
}
