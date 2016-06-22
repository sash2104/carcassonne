#include <fstream>
#include <iostream>
#include "game.hpp"

Game::Game()
  : turn_(0), board_(Board())
{}

Game::~Game() {
  for (Tile* tile : tiles_) { delete tile; }
}

void Game::init() {
  loadTileFile("tiles.txt");
  board_.setPile(tiles_);
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

int Game::loadTileFile(std::string filename) {
  std::ifstream ifs(filename);
  if (ifs.fail())
  {
    std::cerr << filename << "を開けません" << std::endl;
    return -1;
  }
  std::string dummy;
  getline(ifs, dummy); // ignore first line
  std::string edge_type, farm_connection, city_connection, road_connection;
  int tile_id;
  int num_tiles, cloister_flag, pennant_flag;
  ifs >> tile_id >> edge_type >> num_tiles;
  ifs >> farm_connection >> city_connection >> road_connection;
  ifs >> cloister_flag >> pennant_flag;

  while(!ifs.eof()) {
    Tile *tile = new Tile(tile_id, num_tiles,
                          cloister_flag==1, pennant_flag==1);
    tile->setEdgeInfo(edge_type);
    tile->setRoadInfo(road_connection);
    tile->setCityInfo(city_connection);
    tile->setFarmInfo(farm_connection);
    ifs >> tile_id >> edge_type >> num_tiles;
    ifs >> farm_connection >> city_connection >> road_connection;
    ifs >> cloister_flag >> pennant_flag;
    tiles_.push_back(tile);
  }
  return 0;
}
