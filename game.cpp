#include <iostream>
#include "game.hpp"

Game::Game()
  : turn_(0), board_(Board())
{}

Game::~Game() {}

void Game::init() {
  board_.loadTileFile("tiles.txt");
}

void Game::process_turn() {
  std::cerr << turn_ << " " << board_.getCurrentTileId(turn_) << std::endl;
  ++turn_;
}

bool Game::is_game_end() {
  // TODO: ちゃんとした終了判定書く
  return turn_ > 10;
}

void Game::process_game_end() {
  // TODO:
}
