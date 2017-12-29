#ifndef __SCORE_SHEET_HPP__
#define __SCORE_SHEET_HPP__

#include <string>
#include <vector>

enum class MeepleColor;
enum class SegmentType;
class TileFactory;

SegmentType stringToSegmentType(std::string& str);

class TilePlacement {
  public:
    TilePlacement(int x, int y, int rotation) : x_(x), y_(y), rotation_(rotation) {
    }
    int getX() const {
      return x_;
    }
    int getY() const {
      return y_;
    }
    int getRotation() const {
      return rotation_;
    }
  private:
    int x_;
    int y_;
    int rotation_;
};

class MeeplePlacement {
  public:
    MeeplePlacement(SegmentType type, int index) : type_(type), index_(index) {
    }
    SegmentType getSegmentType() const {
      return type_;
    }
    int getSegmentIndex() const {
      return index_;
    }
  private:
    SegmentType type_;
    int index_;
};

enum class PlacementType {
  INITIAL, SKIPPED, REGULAR,
  INVALID // 不正なタイプであることを表す
};

PlacementType stringToPlacementType(std::string& str);

class Placement {
  public:
    Placement(PlacementType type, MeepleColor player, std::string tile_name,
      TilePlacement* tile_placement, MeeplePlacement* meeple_placement)
      : type_(type), player_(player), tile_name_(tile_name),
	tile_placement_(tile_placement), meeple_placement_(meeple_placement) {
    }
    ~Placement() {
      delete tile_placement_;
      if (meeple_placement_ != nullptr) {
	delete meeple_placement_;
      }
    }
    PlacementType getType() const {
      return type_;
    }
    MeepleColor getPlayer() const {
      return player_;
    }
    std::string getTileName() const {
      return tile_name_;
    }
    const TilePlacement* getTilePlacement() const {
      return tile_placement_;
    }
    const MeeplePlacement* getMeeplePlacement() const {
      return meeple_placement_;
    }
    void debugPrint() const {
      std::cout << "Placement[type: ";
      std::cout << static_cast<int>(type_);
      std::cout << ", tileName: " << tile_name_;
      std::cout << ", x: " << tile_placement_->getX() << ", y: " << tile_placement_->getY();
      std::cout << ", rotation: " << tile_placement_->getRotation() << std::endl;
    }
  private:
    PlacementType type_;
    MeepleColor player_;
    std::string tile_name_;
    TilePlacement* tile_placement_;
    MeeplePlacement* meeple_placement_; // nullptrを許す
};

class ScoreSheet {
  public:
    ScoreSheet() : placements_(), players_() {
    }
    ~ScoreSheet() {
      for (auto it = placements_.begin(); it != placements_.end(); it++) {
	Placement* placement = *it;
	delete placement;
      }
    }
    const std::vector<Placement*>* getPlacements() const {
      return &placements_;
    }
    void addPlacement(Placement* placement) {
      placements_.push_back(placement);
    }
    const std::vector<MeepleColor>* getPlayers() const {
      return &players_;
    }
    void addPlayer(MeepleColor player) {
      players_.push_back(player);
    }
  private:
    std::vector<Placement*> placements_;
    std::vector<MeepleColor> players_;
};

// ファイルから棋譜を読み込む
// (主にシンタックスの面で)不正な棋譜の場合はnullptrを返す
ScoreSheet* readScoreSheetFromFile(const std::string& file_name);

// 非合法手順がないかを判定する
// 全て合法な手順の場合だけtrueを返す
bool validateScoreSheet(const TileFactory& tile_factory, const ScoreSheet& score_sheet);

#endif
