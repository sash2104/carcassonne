#ifndef __GAME_CONTEXT_HPP__
#define __GAME_CONTEXT_HPP__

#include <map>
#include <unordered_map>

#include "meeple_color.hpp"
#include "region.hpp"

enum class RegionType;

class GameContext {
  public:
    GameContext(int initial_meeple_n);
    ~GameContext();
    void registerMeeple(MeepleColor color);
    void endTurn();
    void endGame();
    void returnMeeple(MeepleColor color, int n);
    void placeMeeple(MeepleColor color);
    void addPoint(MeepleColor color, RegionType type, int point);
    int getTotalPoint(MeepleColor color);
    int getGainedPoint(MeepleColor color);
    int getGainedPoint(MeepleColor color, RegionType type);
    int getHoldingMeepleCount(MeepleColor color);
    int getReturnedMeepleCount(MeepleColor color);
    int getOnBoardMeepleCount(MeepleColor color);
  private:
    class Container {
      public:
        Container();
        int& totalPoint();
        std::map<int, int>& gainedPoints();
        int& holdingMeepleCount();
        int& returnedMeepleCount();
        int& onBoardMeepleCount();
      private:
        int total_point_;
        std::map<int, int> gained_points_;
        int holding_meeple_count_;
        int returned_meeple_count_;
        int on_board_meeple_count_;
    };
    const int initial_meeple_n_;
    std::map<int, Container*> containers_;
};

#endif
