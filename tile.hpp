#ifndef __TILEINFO_HPP__
#define __TILEINFO_HPP__

#include <string>

struct Segment {
    enum Type {
      ROAD, CITY, CLOISTER, FARM
    };
};

class Tile {
  public:
    Tile(int id, int num_tiles, bool has_cloister, bool has_pennant);
    void setEdgeInfo(std::string edge_type); // edge_id_を計算
    void setRoadInfo(std::string road_connection);
    void setCityInfo(std::string city_connection);
    void setFarmInfo(std::string farm_connection);
  private:
    int id_; // タイルの種類ごとに固有のid
    int edge_id_; // 辺の種類ごとに固有のid (ex. マナカナと三角は同じ)
    int num_tiles_;
    int roads_[4];
    int cities_[4];
    int farms_[9];
    bool has_cloister_;
    bool has_pennant_;
    Segment::Type top_, bottom_, left_, right_;
};

#endif
