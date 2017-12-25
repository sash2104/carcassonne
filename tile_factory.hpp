#ifndef __TILE_FACTORY_HPP__
#define __TILE_FACTORY_HPP__

#include <map>
#include <string>

#include "json.hpp"

using json = nlohmann::json;

class Tile;

// タイルの名前を元にタイルを生成するクラス
// 最初にJSONファイルからタイルの情報を読み込む
class TileFactory {
  public:
    TileFactory();
    ~TileFactory();
    void loadResource(const std::string& resource_file);
    Tile* newFromName(const std::string name, int tile_id) const;
  private:
    class TileParts {
      public:
        TileParts(const std::string name, const int* cities, const int* roads, const int* fields,
	  bool has_cloister, bool* pennants, int pennants_n);
        ~TileParts();
        std::string getName() const;
        const int* getCities() const;
        const int* getRoads() const;
        const int* getFields() const;
        bool getHasCloister() const;
        const bool* getPennants() const;
        int getPennantsNum() const;
        int getCitySegmentNum() const;
        int getRoadSegmentNum() const;
        int getFieldSegmentNum() const;
        void print() const;
      private:
        const std::string name_;
        int cities_[4];
        int roads_[4];
        int fields_[8];
        bool has_cloister_;
        bool* pennants_;
        int pennants_n_;
        int city_segment_n_;
        int road_segment_n_;
        int field_segment_n_;
    };
    TileParts* constractTileParts(json* j);
    std::map<std::string, TileParts*> parts_map_;
};

#endif
