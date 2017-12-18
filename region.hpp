#ifndef __REGION_HPP__
#define __REGION_HPP__

#include <vector>

#include "board.hpp"
#include "game_context.hpp"
#include "segment.hpp"

class Board;
class GameContext;
class Segment;

// 複数のセグメントが繋がってできる領域を表すクラス
// Regionは抽象クラスになっており、具象クラスとして
// CityRegion, CloisterRegion, FieldRegion, RoadRegion
// の4つがある
// それぞれ、得点の計算方法や完成の条件が違っている
class Region {
  public:
    Region(int id, Segment* segment, Board* board);
    virtual ~Region();
    int getId() const;
    Board* getBoard() const;
    void addSegment(Segment* segment);
    const std::vector<Segment*>* getSegments() const;
    bool mergeRegion(Region* region);
    bool isMerged() const;
    void merged();
    // segmentにミープルを置いたときsegmentから呼び出す
    void meepleIsPlacedOnSegment(Segment* segment);
    const std::vector<Segment*>* getMeeplePlacedSegments() const;
    bool meepleIsPlaced() const;
    // 置いているミープルの数が多い色を返す(同数の場合、複数の色を返す)
    void getWinningMeeples(std::vector<MeepleColor>* winning_meeples) const;
    void returnMeeples(GameContext* context);
    void transferPoint(GameContext* context, bool return_meeple);
    bool pointIsTransfered() const;
    virtual bool isCompleted() = 0;
    virtual int calculatePoint() = 0;
    virtual RegionType getType() const = 0;
    void debugPrint() const;
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
    CityRegion(int id, Segment* segment, Board* board);
    bool isCompleted();
    int calculatePoint();
    RegionType getType() const;
  private:
    bool completed_;
};

class CloisterRegion : public Region {
  public:
    CloisterRegion(int id, Segment* segment, Board* board);
    bool isCompleted();
    int calculatePoint();
    RegionType getType() const;
  private:
    bool completed_;
};

class FieldRegion : public Region {
  public:
    FieldRegion(int id, Segment* segment, Board* board);
    bool isCompleted();
    int calculatePoint();
    // テストをしやすくするため
    int calculatePoint(const std::vector<CityRegion*>* city_regions);
    RegionType getType() const;
    bool isAdjacentWith(const CityRegion* city_region);
  private:
    bool completed_;
};

class RoadRegion : public Region {
  public:
    RoadRegion(int id, Segment* segment, Board* board);
    bool isCompleted();
    int calculatePoint();
    RegionType getType() const;
  private:
    bool completed_;
};

#endif
