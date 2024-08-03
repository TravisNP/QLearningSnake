#ifndef ENUMS_H
#define ENUMS_H

#include<iostream>
#include<string>

enum CELL {
  EMPTY,
  SNAKE,
  FRUIT
};

enum DIR {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    UNKNOWN
};

enum REFLECTION {
  NOTHING = 0,
  HORIZONTAL,
  VERTICAL,
  DIAGONAL,
  HORIZONTAL_VERTICAL,
  HORIZONTAL_DIAGONAL,
  VERTICAL_DIAGONAL,
  HORIZONTAL_VERTICAL_DIAGONAL
};

enum REWARDS {
  REWARD_DEATH = -20,
  REWARD_NOTHING = -1,
  REWARD_FRUIT = 5,
  REWARD_FINISH = 20,
};

std::ostream& operator<<(std::ostream& os, const CELL& cell);
std::ostream& operator<<(std::ostream& os, const DIR& cell);
std::ostream& operator<<(std::ostream& os, const REFLECTION& cell);

#endif