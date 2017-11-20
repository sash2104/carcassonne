#ifndef __TILEINFO_HPP__
#define __TILEINFO_HPP__

#include <vector>

#include "segment.hpp"

class Segment;

enum class ESegment{
  ROAD, CITY, CLOISTER, FARM
};

enum class BorderType {
  ROAD, CITY, CLOISTER, FIELD
};

class Tile {
  public:
    Tile(int id, char* name, BorderType* borderTypes, int* cities, int* roads, int* fields);
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
    const int* getCities() const;
    const int* getRoads() const;
    const int* getFields() const;
    const Segment* getCitySegmentOfDirection(int direction) const;
    const Segment* getFieldSegmentOfDirection(int direction) const;
    const Segment* getRoadSegmentOfDirection(int direction) const;
    const std::vector<Segment*>* getCitySegments() const;
    const std::vector<Segment*>* getFieldSegments() const;
    const std::vector<Segment*>* getRoadSegments() const;
    Segment* getCloisterSegment();
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
};

#endif
