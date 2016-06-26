#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "board.hpp"
#include "common.hpp"

// TODO: virturlクラスにして、継承したクラスで最低限の行動をするプレーヤーを実装する
class Player {
  public:
    Player(int id);
    ~Player();
    //盤面の情報から次にタイルを置く場所を決める
    Pos determineTilePos(const Board& board);
    //盤面と今置いたタイルの位置の情報からミープルを置くか, 置くならどこかを決める
    int determineMeeplePlacing(const Board& board, const Pos &p);
  private:
    int id_; // player id
};

#endif

