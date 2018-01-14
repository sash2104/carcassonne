#include <iostream>

#include "game.hpp"
#include "player.hpp"
#include "tile.hpp"

// TODO : n_players_ とタイルの種類はconfigファイルから読み込む
Game::Game()
  : turn_(0), n_players_(2), board_(72, 7)
{
  //board_.setPile(tiles_);
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
  ++turn_;
}

bool Game::is_game_end() {
  // TODO: ちゃんとした終了判定書く
  return turn_ > 10;
}

void Game::process_game_end() {
  // TODO:
}
