#ifndef __TILEINFO_HPP__
#define __TILEINFO_HPP__

struct Segment {
    enum Type {
      ROAD, CITY, CLOISTER, FARM
    };
};

class Tile {
  public:
    Tile(int id);
  private:
    int id_;
    int num_tiles_;
    bool has_pennant_;
    bool has_cloister_;
    Segment::Type top_, bottom_, left_, right_;
};

#endif
