#ifndef __SEGMENT_HPP__
#define __SEGMENT_HPP__

#include "region.hpp"
#include "tile.hpp"

class Region;

enum class SegmentType {
  CITY, CLOISTER, ROAD, FIELD
};

class Segment {
  public:
    Segment(int index, SegmentType type, bool has_pennant);
    int getIndex() const;
    SegmentType getType() const;
    Tile* getTile() const;
    void setTile(Tile* tile);
    Region* getRegion() const;
    void setRegion(Region* region);
    bool hasPennant() const;
    bool isAdjacentTo(int direction) const;
    void placeMeeple(int meepleColor);
  private:
    int index_;
    SegmentType type_;
    Tile* tile_;
    Region* region_;
    bool has_pennant_;
};

#endif
