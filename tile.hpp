#ifndef __TILEINFO_HPP__
#define __TILEINFO_HPP__

#include <vector>

#include "segment.hpp"

class Segment;

enum class BorderType {
  ROAD, CITY, CLOISTER, FIELD
};

class Tile {
  public:
    Tile(int id, char* name, BorderType* borderTypes, int* cities, int* roads, int* fields,
       std::vector<Segment*>* city_segments, std::vector<Segment*>* field_segments,
       std::vector<Segment*>* road_segments, Segment* cloister_segment);
    Tile(int id); // for testing
    ~Tile();
    int getId() const;
    const char* getName() const;
    int getX() const;
    int getY() const;
    int getRotation() const;
    void setX(int x);
    void setY(int y);
    void setRotation(int rotation);
    BorderType getBorderType(int direction);
    BorderType getBorderType(int direction, int rotation);
    const int* getCities() const;
    const int* getRoads() const;
    const int* getFields() const;
    Segment* getCitySegmentOfDirection(int direction) const;
    Segment* getFieldSegmentOfDirection(int direction) const;
    Segment* getRoadSegmentOfDirection(int direction) const;
    const std::vector<Segment*>* getCitySegments() const;
    const std::vector<Segment*>* getFieldSegments() const;
    const std::vector<Segment*>* getRoadSegments() const;
    Segment* getCloisterSegment();
    // rotation回転した状態のtileをこのタイルの上に置けるかどうか
    bool canTopAdjacentWith(Tile* tile, int rotation);
    // rotation回転した状態のtileをこのタイルの右に置けるかどうか
    bool canRightAdjacentWith(Tile* tile, int rotation);
    // rotation回転した状態のtileをこのタイルの下に置けるかどうか
    bool canBottomAdjacentWith(Tile* tile, int rotation);
    // rotation回転した状態のtileをこのタイルの左に置けるかどうか
    bool canLeftAdjacentWith(Tile* tile, int rotation);
    // rotation回転した状態のtileをこのタイルのdirectionの位置に置けるかどうか
    bool canAdjacentWith(int direction, Tile* tile, int rotation);
    bool isTwoSegmentAdjacent(int field_segment_index, int city_segment_index);
  private:
    int id_; // タイルごとに固有のid
    char* name_;
    int x_;
    int y_;
    int rotation_;
    BorderType border_types_[4];
    int cities_[4];
    int roads_[4];
    int fields_[8];
    std::vector<Segment*>* city_segments_;
    std::vector<Segment*>* field_segments_;
    std::vector<Segment*>* road_segments_;
    Segment* cloister_segment_; // nullptrも許す
    bool isAdjacentDirection(int field_d, int city_d);
};

#endif
