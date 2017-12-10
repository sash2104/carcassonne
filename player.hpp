#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "board.hpp"

// TODO: virturlクラスにして、継承したクラスで最低限の行動をするプレーヤーを実装する
class Player {
  public:
    Player(int id);
    ~Player();
  private:
    int id_; // player id
};

#endif

