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
    void setEdgeID(std::string edge_type);
  private:
    int id_; // タイルの種類ごとに固有のid
    int edge_id_; // 辺の種類ごとに固有のid (ex. マナカナと三角は同じ)
    int num_tiles_;
    bool has_cloister_;
    bool has_pennant_;
    Segment::Type top_, bottom_, left_, right_;
};

#endif
