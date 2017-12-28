#ifndef __TILE_POSITION_MAP_HPP__
#define __TILE_POSITION_MAP_HPP__

#include <set>

class Tile;

class BoardPosition {
  public:
    BoardPosition(int x, int y);
    BoardPosition(const BoardPosition& that);
    int getX() const;
    int getY() const;
    bool operator<(const BoardPosition& that) const;
  private:
    int x_;
    int y_;
};

// 置かれたタイルの位置を管理するクラス
// 位置のみに関心を置き、隣り合ったタイルの境界が一致しているかどうかは気にしない
class TilePositionMap {
  public:
    TilePositionMap(int tile_n);
    ~TilePositionMap();
    void placeTile(Tile* tile, int x, int y);
    bool isTilePlaced(int x, int y) const;
    Tile* getPlacedTile(int x, int y) const;
    // タイルを置くことが可能な位置を全て返す。
    // 置くことが可能な位置とは次の２つの条件を満たす位置
    //   - その位置にはタイルが置かれていない
    //   - その位置の上か下か右か左のいずれかの位置にタイルが置かれている
    const std::set<BoardPosition>* getPlacablePositions() const;
    // (x, y)にタイルを置くことが可能ならtrueを返す。
    // 置くことが可能の意味はgetPlacablePositionsと同じ。
    bool isPlacablePosition(int x, int y) const;
    Tile* undo(int x, int y);
  private:
    int size_;
    int shift_;
    Tile** tiles_;
    std::set<BoardPosition> placables_;
    void checkAndAddPlacablePosition(int x, int y);
    void checkAndRemovePlacablePosition(int x, int y);
    int convertToIndex(int x, int y) const;
};

#endif
