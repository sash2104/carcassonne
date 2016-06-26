#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <vector>

#include "board.hpp"
#include "player.hpp"
#include "tile.hpp"

// TODO : 設定ファイル(最低限, 参加プレーヤー数と使うタイルセットのファイルの情報)の作成と読み込み

class Game {
  public:
    Game(); // 先手後手, 使うタイル, タイルの順番などを決める
    ~Game();
    void process_turn(); // 一枚タイルを引く, タイルを置く, ミープルを置く, 得点計算, ミープルの回収
    bool is_game_end(); // ゲームの終了判定. 残りのタイルがなくなったら終了
    void process_game_end(); // ゲーム終了時の処理. 得点計算と勝者決定
    int loadTileFile(std::string filename); // タイルの情報が書かれたファイルを読み込みtiles_に格納
  private:
    int turn_; // ターンは0から始まる
    int n_players_; // ゲームに参加するプレーヤーの数. 今は二人のみを想定.
    Board board_; // 配置済みのタイルと未設置のタイルの山をそれぞれ管理
    std::vector<Player*> players_; // ゲームに参加するプレーヤーのリスト
    std::vector<Tile*> tiles_; // 使う全種類のタイルへのポインタを格納

};

#endif
