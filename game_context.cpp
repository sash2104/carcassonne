#include <cassert>
#include <map>

#include "game_context.hpp"
#include "meeple_color.hpp"
#include "region.hpp"


GameContext::GameContext(int initial_meeple_n) : containers_(), initial_meeple_n_(initial_meeple_n) {
}

GameContext::~GameContext() {
  for (auto it = containers_.begin(); it != containers_.end();) {
    Container* c = it->second;
    containers_.erase(it++);
    delete c;
  }
}

void GameContext::registerMeeple(MeepleColor color) {
  int k = static_cast<int>(color);
  if (containers_.find(k) != containers_.end()) {
    return;
  }
  Container* c = new Container();
  c->holdingMeepleCount() = initial_meeple_n_;
  containers_[k] = c;
}

void GameContext::endTurn() {
  for (auto it = containers_.begin(); it != containers_.end(); it++) {
    Container* c = it->second;
    c->holdingMeepleCount() += c->returnedMeepleCount();
    c->returnedMeepleCount() = 0;
    for (auto it2 = c->gainedPoints().begin(); it2 != c->gainedPoints().end(); it2++) {
      int point = it2->second;
      c->totalPoint() += point;
      it2->second = 0;
    }
  }
}

void GameContext::endGame() {
  endTurn();
}

void GameContext::returnMeeple(MeepleColor color, int n) {
  int k = static_cast<int>(color);
  auto it = containers_.find(k);
  assert(it != containers_.end());
  Container* c = (*it).second;
  assert(c->onBoardMeepleCount() >= n);
  c->onBoardMeepleCount() -= n;
  c->returnedMeepleCount() += n;
}

void GameContext::placeMeeple(MeepleColor color) {
  int k = static_cast<int>(color);
  auto it = containers_.find(k);
  assert(it != containers_.end());
  Container* c = (*it).second;
  assert(c->holdingMeepleCount() > 0);
  c->holdingMeepleCount() -= 1;
  c->onBoardMeepleCount() += 1;
}

void GameContext::addPoint(MeepleColor color, RegionType type, int point) {
  int k = static_cast<int>(color);
  auto it = containers_.find(k);
  assert(it != containers_.end());
  Container* c = (*it).second;
  c->gainedPoints()[static_cast<int>(type)] += point;
}

int GameContext::getTotalPoint(MeepleColor color) const {
  int k = static_cast<int>(color);
  auto it = containers_.find(k);
  assert(it != containers_.end());
  Container* c = (*it).second;
  return c->totalPoint();
}

int GameContext::getGainedPoint(MeepleColor color) const {
  int k = static_cast<int>(color);
  auto it = containers_.find(k);
  assert(it != containers_.end());
  Container* c = (*it).second;
  int point = 0;
  for (auto it = c->gainedPoints().begin(); it != c->gainedPoints().end(); it++) {
    point += it->second;
  }
  return point;
}

int GameContext::getGainedPoint(MeepleColor color, RegionType type) const {
  int k = static_cast<int>(color);
  auto it = containers_.find(k);
  assert(it != containers_.end());
  Container* c = (*it).second;
  return c->gainedPoints()[static_cast<int>(type)];
}

int GameContext::getHoldingMeepleCount(MeepleColor color) const {
  int k = static_cast<int>(color);
  auto it = containers_.find(k);
  assert(it != containers_.end());
  Container* c = (*it).second;
  return c->holdingMeepleCount();
}

int GameContext::getReturnedMeepleCount(MeepleColor color) const {
  int k = static_cast<int>(color);
  auto it = containers_.find(k);
  assert(it != containers_.end());
  Container* c = (*it).second;
  return c->returnedMeepleCount();
}

int GameContext::getOnBoardMeepleCount(MeepleColor color) const {
  int k = static_cast<int>(color);
  auto it = containers_.find(k);
  assert(it != containers_.end());
  Container* c = (*it).second;
  return c->onBoardMeepleCount();
}

GameContext::Container::Container() : gained_points_(), total_point_(0),
  holding_meeple_count_(0), returned_meeple_count_(0), on_board_meeple_count_(0) {
  // TODO: use for statement
  gained_points_[static_cast<int>(RegionType::CITY)] = 0;
  gained_points_[static_cast<int>(RegionType::CLOISTER)] = 0;
  gained_points_[static_cast<int>(RegionType::FIELD)] = 0;
  gained_points_[static_cast<int>(RegionType::ROAD)] = 0;
}

int& GameContext::Container::totalPoint() {
  return total_point_;
}

std::map<int, int>& GameContext::Container::gainedPoints() {
  return gained_points_;
}

int& GameContext::Container::holdingMeepleCount() {
  return holding_meeple_count_;
}

int& GameContext::Container::returnedMeepleCount() {
  return returned_meeple_count_;
}

int& GameContext::Container::onBoardMeepleCount() {
  return on_board_meeple_count_;
}
