#ifndef __SEGMENT_HPP__
#define __SEGMENT_HPP__

#include "meeple_color.hpp"
#include "region.hpp"
#include "tile.hpp"

class Region;
class Tile;

enum class SegmentType {
  CITY, CLOISTER, ROAD, FIELD
};

// タイル中でミープルを置く対象となる個々の場所を表すクラス
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
    void placeMeeple(MeepleColor meeple);
    MeepleColor getPlacedMeeple() const;
    bool meepleIsPlaced() const;
  private:
    int index_;
    SegmentType type_;
    Tile* tile_;
    Region* region_;
    bool has_pennant_;
    MeepleColor placed_meeple_;
};

#endif
