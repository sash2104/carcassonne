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


// 複数のセグメントが繋がってできる領域を表すクラス
// Regionは抽象クラスになっており、具象クラスとして
// CityRegion, CloisterRegion, FieldRegion, RoadRegion
// の4つがある
// それぞれ、得点の計算方法や完成の条件が違っている
class Region {
  public:
    class SegmentIterator;
    class MeeplePlacedSegmentIterator;
    Region(int id, Segment* segment, Board* board);
    virtual ~Region();
    int getId() const;
    Board* getBoard() const;
    void addSegment(Segment* segment);
    void undoAddSegment(Segment* segment);
    SegmentIterator segmentBegin() const;
    SegmentIterator segmentEnd() const;
    bool mergeRegion(Region* region);
    void undoMergeRegion(Region* region);
    bool isMerged() const;
    // segmentにミープルを置いたときsegmentから呼び出す
    void meepleIsPlacedOnSegment(Segment* segment);
    // segmentからミープルを除くときにsegmentから呼び出す
    void meepleIsUnplacedOnSegment(Segment* segment);
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
    class SegmentIterator {
      public:
        friend Region;
        SegmentIterator(const SegmentIterator& iter);
        SegmentIterator& operator++();
        Segment* operator*();
        bool operator==(const SegmentIterator& iter);
        bool operator!=(const SegmentIterator& iter);
      private:
        SegmentIterator(const Region* start, const Region* current,
	  std::vector<Segment*>::const_iterator iter);
        void advanceToActualNext();
        void setToEnd();
        const Region* start_;
        const Region* current_;
        std::vector<Segment*>::const_iterator iter_;
    };
    friend SegmentIterator;
  protected:
    void appendChild(Region* region);
    void removeLastChild();
    int id_;
    Board* board_;
    std::vector<Segment*> segments_;
    int meeple_placed_count_;
    std::vector<MeepleColor> winning_meeples_;
    Region* parent_;
    Region* first_child_;
    Region* last_child_;
    Region* prev_sibling_;
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
