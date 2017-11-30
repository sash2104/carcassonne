#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "json.hpp"
#include "segment.hpp"
#include "tile.hpp"
#include "tile_factory.hpp"

using json = nlohmann::json;

bool TileFactory::loadResource(const char* resource_file) {
  std::ifstream i(resource_file);
  json j;
  i >> j;
  for (json::iterator it = j.begin(); it != j.end(); ++it) {
    TileParts* parts = constractTileParts(&(*it));
    parts_map_[parts->name] = parts;
  }
  return true;
}

TileParts* TileFactory::constractTileParts(json* j) {
  TileParts* parts = new TileParts();
  int i = 0;
  for (json::iterator it = (*j)["cities"].begin(); it != (*j)["cities"].end(); ++it) {
    if (i < 4) {
      parts->cities[i] = *it;
    }
    ++i;
  }
  i = 0;
  for (json::iterator it = (*j)["roads"].begin(); it != (*j)["roads"].end(); ++it) {
    if (i < 4) {
      parts->roads[i] = *it;
    }
    ++i;
  }
  i = 0;
  for (json::iterator it = (*j)["fields"].begin(); it != (*j)["fields"].end(); ++it) {
    if (i < 8) {
      parts->fields[i] = *it;
    }
    ++i;
  }
  i = 0;
  int n = (*j)["pennants"].size();
  parts->pennants = new bool[n];
  for (json::iterator it = (*j)["pennants"].begin(); it != (*j)["pennants"].end(); ++it) {
    if (i < 8) {
      parts->pennants[i] = *it;
    }
    ++i;
  }
  parts->has_cloister = (*j)["hasCloister"];
  parts->name = (*j)["name"];
  return parts;
}

Tile* TileFactory::newFromName(const std::string* name, int tile_id) {
  auto it = parts_map_.find(*name);
  if (it == parts_map_.end()) {
    return nullptr;
  }
  TileParts* parts = (*it).second;
  int city_segment_n = 0;
  for (int i = 0; i < 4; i++) {
    if (city_segment_n < parts->cities[i] + 1) {
      city_segment_n = parts->cities[i] + 1;
    }
  }
  std::vector<Segment*>* city_segments = new std::vector<Segment*>();
  for (int i = 0; i < city_segment_n; i++) {
    city_segments->push_back(new Segment(i, SegmentType::CITY, parts->pennants[i]));
  }

  int road_segment_n = 0;
  for (int i = 0; i < 4; i++) {
    if (road_segment_n < parts->roads[i] + 1) {
      road_segment_n = parts->roads[i] + 1;
    }
  }
  std::vector<Segment*>* road_segments = new std::vector<Segment*>();
  for (int i = 0; i < road_segment_n; i++) {
    road_segments->push_back(new Segment(i, SegmentType::ROAD, false));
  }

  int field_segment_n = 0;
  for (int i = 0; i < 8; i++) {
    if (field_segment_n < parts->fields[i] + 1) {
      field_segment_n = parts->fields[i] + 1;
    }
  }
  std::vector<Segment*>* field_segments = new std::vector<Segment*>();
  for (int i = 0; i < field_segment_n; i++) {
    field_segments->push_back(new Segment(i, SegmentType::FIELD, false));
  }

  Segment* cloister_segment = nullptr;
  if (parts->has_cloister) {
    cloister_segment = new Segment(0, SegmentType::CLOISTER, false);
  }

  BorderType border_types[4];
  for (int d = 0; d < 4; d++) {
    if (parts->cities[d] != -1) {
      border_types[d] = BorderType::CITY;
    } else if (parts->roads[d] != -1) {
      border_types[d] = BorderType::ROAD;
    } else {
      border_types[d] = BorderType::FIELD;
    }
  }

  const char* str = name->c_str();
  char* s = new char[name->size() + 1];
  for (int i = 0; i < name->size() + 1; i++) {
    s[i] = str[i];
  }

  return new Tile(tile_id, s, border_types, parts->cities, parts->roads, parts->fields,
    city_segments, field_segments, road_segments, cloister_segment);
}

void TileParts::print() {
  std::cout << name << std::endl;
  std::cout << "cities: ";
  for (int i = 0; i < 4; i++) {
    std::cout << cities[i] << " ";
  }
  std::cout << std::endl;
  std::cout << "fields: ";
  for (int i = 0; i < 8; i++) {
    std::cout <<fields[i] << " ";
  }
  std::cout << std::endl;
  std::cout << "pennants: ";
  for (int i = 0; i < 2; i++) {
    std::cout << pennants[i] << " ";
  }
  std::cout << std::endl;
  std::cout << has_cloister << std::endl;
}
