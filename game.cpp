#include <fstream>
#include <iostream>
#include "game.hpp"

// TODO : n_players_ とタイルの種類はconfigファイルから読み込む
Game::Game()
  : turn_(0), n_players_(2), board_(72)
{
  board_.setPile(tiles_);
  for (int pid = 0; pid < n_players_; ++pid) {
    players_.push_back(new Player(pid));
  }
}

Game::~Game() {
  for (Tile *tile : tiles_) { delete tile; }
  for (Player *player : players_) { delete player; }
}

void Game::process_turn() {
  std::cerr << turn_ << std::endl;
  // TODO: turn = 0の時だけ別処理に?
  // TODO: getActivePlayerIDという関数にする
  int active_player_id = turn_ % n_players_;
  Player * active_player = players_[active_player_id];
  Pos tile_pos = active_player->determineTilePos(board_);
  const Tile* tile = board_.getCurrentTile(turn_);
  // TODO: dirを0以外にも. 入力をx, yでなくposに
  board_.placeTile(tile, 0, tile_pos.x, tile_pos.y);
  board_.printField();
  ++turn_;
}

bool Game::is_game_end() {
  // TODO: ちゃんとした終了判定書く
  return turn_ > 10;
}

void Game::process_game_end() {
  // TODO:
}
