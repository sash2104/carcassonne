#include <cassert>
#include <iostream>
#include <vector>

#include "segment.hpp"
#include "tile.hpp"
#include "utils.hpp"

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
  name_ = nullptr;
  city_segments_ = new std::vector<Segment*>();
  field_segments_ = new std::vector<Segment*>();
  road_segments_ = new std::vector<Segment*>();
  cloister_segment_ = nullptr;
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
  return getBorderType(direction, getRotation());
}

BorderType Tile::getBorderType(int direction, int rotation) {
  assert(direction >= 0 && direction < 4);
  assert(rotation >= 0 && rotation < 4);
  return border_types_[modBy4(direction - rotation)];
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

Segment* Tile::getCitySegmentOfDirection(int direction) const {
  assert(direction >= 0 && direction < 4);
  int index = cities_[modBy4(direction - rotation_)];
  return (index == -1) ? nullptr : city_segments_->at(index);
}

Segment* Tile::getFieldSegmentOfDirection(int direction) const {
  assert(direction >= 0 && direction < 8);
  int index = fields_[modBy8(direction - rotation_ * 2)];
  return (index == -1) ? nullptr : field_segments_->at(index);
}

Segment* Tile::getRoadSegmentOfDirection(int direction) const {
  assert(direction >= 0 && direction < 4);
  int index = roads_[modBy4(direction - rotation_)];
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

bool Tile::canTopAdjacentWith(Tile* tile, int rotation) {
  return canAdjacentWith(0, tile, rotation);
}

bool Tile::canRightAdjacentWith(Tile* tile, int rotation) {
  return canAdjacentWith(1, tile, rotation);
}

bool Tile::canBottomAdjacentWith(Tile* tile, int rotation) {
  return canAdjacentWith(2, tile, rotation);
}

bool Tile::canLeftAdjacentWith(Tile* tile, int rotation) {
  return canAdjacentWith(3, tile, rotation);
}

bool Tile::canAdjacentWith(int direction, Tile* tile, int rotation) {
  BorderType my_border_type = getBorderType(direction);
  BorderType your_border_type = tile->getBorderType(modBy4(direction + 2), rotation);
  return my_border_type == your_border_type;
}

bool Tile::isTwoSegmentAdjacent(int field_segment_index, int city_segment_index) {
  assert(field_segment_index >= 0 && field_segment_index < field_segments_->size());
  assert(city_segment_index >= 0 && city_segment_index < city_segments_->size());
  for (int field_d = 0; field_d < 8; field_d++) {
    if (fields_[field_d] != field_segment_index) {
      continue;
    }
    for (int city_d = 0; city_d < 4; city_d++) {
      if (cities_[city_d] != city_segment_index) {
	continue;
      }
      if (isAdjacentDirection(field_d, city_d)) {
	return true;
      }
    }
  }
  return false;
}

bool Tile::isAdjacentDirection(int field_d, int city_d) {
  assert(field_d >= 0 && field_d < 8);
  assert(city_d >= 0 && city_d < 4);
  int city_d1 = city_d * 2;
  int city_d2 = city_d * 2 + 1;
  return modBy8(city_d1 - 1) == field_d || modBy8(city_d2 + 1) == field_d;
}

void Tile::print_info() {
  std::cout << "{name: " << name_ << ", ";
  std::cout << "cities: [";
  for (int i = 0; i < 4; i++) {
    std::cout << cities_[i] << ", ";
  }
  std::cout << "]";
  std::cout << "}" << std::endl;
}
