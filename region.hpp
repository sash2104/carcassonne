#ifndef __REGION_HPP__
#define __REGION_HPP__

#include <vector>

#include "board.hpp"
#include "game_context.hpp"
#include "segment.hpp"

class Board;
class GameContext;
class Segment;

enum class RegionType {
  CITY, CLOISTER, ROAD, FIELD
};

class Region {
  public:
    Region(int id, Board* board);
    virtual ~Region();
    int getId() const;
    Board* getBoard() const;
    void addSegment(Segment* segment);
    const std::vector<Segment*>* getSegments() const;
    bool mergeRegion(Region* region);
    bool isMerged() const;
    void merged();
    void meepleIsPlacedOnSegment(Segment* segment);
    const std::vector<Segment*>* getMeeplePlacedSegments() const;
    bool meepleIsPlaced() const;
    void getWinningMeeples(std::vector<MeepleColor>* winning_meeples) const;
    void returnMeeples(GameContext* context);
    void transferPoint(GameContext* context, bool return_meeple);
    bool pointIsTransfered() const;
    virtual bool isCompleted() = 0;
    virtual int calculatePoint() = 0;
    virtual RegionType getType() const = 0;
  protected:
    int id_;
    Board* board_;
    std::vector<Segment*> segments_;
    std::vector<Segment*> meeple_placed_segments_;
    std::vector<MeepleColor> winning_meeples_;
    bool merged_;
    bool point_transfered_;
};

class CityRegion : public Region {
  public:
    CityRegion(int id, Board* board);
    bool isCompleted();
    int calculatePoint();
    RegionType getType() const;
  private:
    bool completed_;
};

class CloisterRegion : public Region {
  public:
    CloisterRegion(int id, Board* board);
    bool isCompleted();
    int calculatePoint();
    RegionType getType() const;
  private:
    bool completed_;
};

class FieldRegion : public Region {
  public:
    FieldRegion(int id, Board* board);
    bool isCompleted();
    int calculatePoint();
    RegionType getType() const;
    bool isAdjacentWith(const CityRegion* city_region);
  private:
    bool completed_;
};

class RoadRegion : public Region {
  public:
    RoadRegion(int id, Board* board);
    bool isCompleted();
    int calculatePoint();
    RegionType getType() const;
  private:
    bool completed_;
};

#endif
