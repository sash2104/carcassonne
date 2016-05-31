#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <string>

class Board {
  public:
    Board();
    int load_tile_file(std::string filename);
};

#endif
