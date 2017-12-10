#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "json.hpp"
#include "segment.hpp"
#include "tile.hpp"
#include "tile_factory.hpp"

using json = nlohmann::json;

void TileFactory::loadResource(const char* resource_file) {
  std::ifstream i(resource_file);
  json j;
  i >> j;
  for (json::iterator it = j.begin(); it != j.end(); ++it) {
    TileParts* parts = constractTileParts(&(*it));
    parts_map_[parts->getName()] = parts;
  }
}

TileFactory::TileFactory() : parts_map_() {
}

TileFactory::~TileFactory() {
  for (auto it = parts_map_.begin(); it != parts_map_.end();) {
    TileParts* parts = it->second;
    parts_map_.erase(it++);
    delete parts;
  }
}

TileFactory::TileParts* TileFactory::constractTileParts(json* j) {
  int i = 0;
  int cities[4];
  for (json::iterator it = (*j)["cities"].begin(); it != (*j)["cities"].end(); ++it) {
    if (i < 4) {
      cities[i] = *it;
    }
    ++i;
  }
  i = 0;
  int roads[4];
  for (json::iterator it = (*j)["roads"].begin(); it != (*j)["roads"].end(); ++it) {
    if (i < 4) {
      roads[i] = *it;
    }
    ++i;
  }
  i = 0;
  int fields[8];
  for (json::iterator it = (*j)["fields"].begin(); it != (*j)["fields"].end(); ++it) {
    if (i < 8) {
      fields[i] = *it;
    }
    ++i;
  }
  i = 0;
  int pennants_n = (*j)["pennants"].size();
  bool* pennants = new bool[pennants_n];
  for (json::iterator it = (*j)["pennants"].begin(); it != (*j)["pennants"].end(); ++it) {
    if (i < 8) {
      pennants[i] = *it;
    }
    ++i;
  }
  bool has_cloister = (*j)["hasCloister"];
  std::string name = (*j)["name"];
  TileParts* parts = new TileParts(name, cities, roads, fields, has_cloister, pennants, pennants_n);
  delete[] pennants;
  return parts;
}

Tile* TileFactory::newFromName(const std::string name, int tile_id) {
  auto it = parts_map_.find(name);
  if (it == parts_map_.end()) {
    return nullptr;
  }
  TileParts* parts = (*it).second;
  std::vector<Segment*>* city_segments = new std::vector<Segment*>();
  for (int i = 0; i < parts->getCitySegmentNum(); i++) {
    city_segments->push_back(new Segment(i, SegmentType::CITY, parts->getPennants()[i]));
  }
  std::vector<Segment*>* road_segments = new std::vector<Segment*>();
  for (int i = 0; i < parts->getRoadSegmentNum(); i++) {
    road_segments->push_back(new Segment(i, SegmentType::ROAD, false));
  }
  std::vector<Segment*>* field_segments = new std::vector<Segment*>();
  for (int i = 0; i < parts->getFieldSegmentNum(); i++) {
    field_segments->push_back(new Segment(i, SegmentType::FIELD, false));
  }

  Segment* cloister_segment = nullptr;
  if (parts->getHasCloister()) {
    cloister_segment = new Segment(0, SegmentType::CLOISTER, false);
  }

  BorderType border_types[4];
  for (int d = 0; d < 4; d++) {
    if (parts->getCities()[d] != -1) {
      border_types[d] = BorderType::CITY;
    } else if (parts->getRoads()[d] != -1) {
      border_types[d] = BorderType::ROAD;
    } else {
      border_types[d] = BorderType::FIELD;
    }
  }

  return new Tile(tile_id, name, border_types, parts->getCities(), parts->getRoads(), parts->getFields(),
    city_segments, field_segments, road_segments, cloister_segment);
}

TileFactory::TileParts::TileParts(const std::string name,
  const int* cities, const int* roads, const int* fields,
  bool has_cloister, bool* pennants, int pennants_n) : name_(name), has_cloister_(has_cloister), pennants_n_(pennants_n) {
  for (int i = 0; i < 4; i++) {
    cities_[i] = cities[i];
    roads_[i] = roads[i];
    fields_[2 * i] = fields[2 * i];
    fields_[2 * i + 1] = fields[2 * i + 1];
  }
  pennants_ = new bool[pennants_n_];
  for (int i = 0; i < pennants_n_; i++) {
    pennants_[i] = pennants[i];
  }
  city_segment_n_ = 0;
  for (int i = 0; i < 4; i++) {
    if (city_segment_n_ < cities_[i] + 1) {
      city_segment_n_ = cities_[i] + 1;
    }
  }
  road_segment_n_ = 0;
  for (int i = 0; i < 4; i++) {
    if (road_segment_n_ < roads_[i] + 1) {
      road_segment_n_ = roads_[i] + 1;
    }
  }
  field_segment_n_ = 0;
  for (int i = 0; i < 8; i++) {
    if (field_segment_n_ < fields_[i] + 1) {
      field_segment_n_ = fields_[i] + 1;
    }
  }
}

TileFactory::TileParts::~TileParts() {
  delete[] pennants_;
}

inline std::string TileFactory::TileParts::getName() const {
  return name_;
}

inline const int* TileFactory::TileParts::getCities() const {
  return cities_;
}

inline const int* TileFactory::TileParts::getRoads() const {
  return roads_;
}

inline const int* TileFactory::TileParts::getFields() const {
  return fields_;
}

inline bool TileFactory::TileParts::getHasCloister() const {
  return has_cloister_;
}

inline const bool* TileFactory::TileParts::getPennants() const {
  return pennants_;
}

inline int TileFactory::TileParts::getPennantsNum() const {
  return pennants_n_;
}

inline int TileFactory::TileParts::getCitySegmentNum() const {
  return city_segment_n_;
}

inline int TileFactory::TileParts::getRoadSegmentNum() const {
  return road_segment_n_;
}

inline int TileFactory::TileParts::getFieldSegmentNum() const {
  return field_segment_n_;
}

void TileFactory::TileParts::print() const {
  std::cout << name_ << std::endl;
  std::cout << "cities: ";
  for (int i = 0; i < 4; i++) {
    std::cout << cities_[i] << " ";
  }
  std::cout << std::endl;
  std::cout << "fields: ";
  for (int i = 0; i < 8; i++) {
    std::cout <<fields_[i] << " ";
  }
  std::cout << std::endl;
  std::cout << "pennants: ";
  for (int i = 0; i < pennants_n_; i++) {
    std::cout << pennants_[i] << " ";
  }
  std::cout << std::endl;
  std::cout << has_cloister_ << std::endl;
}
