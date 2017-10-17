#include "city.hpp"

City::City(const Tile& tile, int direction)
  : top_(NULL_ID), right_(NULL_ID), bottom_(NULL_ID), left_(NULL_ID),
    north_(nullptr), east_(nullptr), west_(nullptr), south_(nullptr),
    tr_n_(ECityRelation::NONE), tr_e_(ECityRelation::NONE),
    tr_w_(ECityRelation::NONE), tr_s_(ECityRelation::NONE)
{
  // TODO : Tileの種類と向きに応じてtop_, right_, bottom_, left_とtr_x_をセット
}
