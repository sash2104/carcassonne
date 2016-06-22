#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "board.hpp"

class Game {
  public:
    Game();
    ~Game();
    void init(); // 先手後手, 使うタイル, タイルの順番などを決める
    void process_turn(); // 一枚タイルを引く, タイルを置く, ミープルを置く, 得点計算, ミープルの回収
    bool is_game_end(); // ゲームの終了判定. 残りのタイルがなくなったら終了
    void process_game_end(); // ゲーム終了時の処理. 得点計算と勝者決定
  private:
    int turn_;
    Board board_;

};

#endif

