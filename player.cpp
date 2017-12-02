#include <random>
#include "player.hpp"

std::random_device rnd;     // 非決定的な乱数生成器を生成
std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
std::uniform_int_distribution<> rand10(0, 10);        // [0, 99] 範囲の一様乱数

Player::Player(int id) : id_(id) {}

Player::~Player() {}
