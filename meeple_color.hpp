#ifndef __MEEPLE_COLOR_HPP__
#define __MEEPLE_COLOR_HPP__

enum class MeepleColor {
  RED = 0,         /* 先攻 */
  GREEN = 1,       /* 後攻 */
  NOT_PLACED = 2   /* 何も置かれてないことを示すための"色" */
};

#endif
