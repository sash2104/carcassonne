#ifndef __TILE_FACTORY_HPP__
#define __TILE_FACTORY_HPP__

#include <map>
#include <string>

#include "json.hpp"
#include "tile.hpp"

using json = nlohmann::json;

class TileParts {
  public:
    std::string name;
    int cities[4];
    int roads[4];
    int fields[8];
    bool has_cloister;
    bool* pennants;
    void print();
};

class TileFactory {
  public:
    bool loadResource(const char* resource_file);
    Tile* newFromName(const std::string* name, int tile_id);
  private:
    TileParts* constractTileParts(json* j);
    std::map<std::string, TileParts*> parts_map_;
};

#endif
