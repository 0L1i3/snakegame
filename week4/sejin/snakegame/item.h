#ifndef ITEM_H
#define ITEM_H

#include <utility>
#include <vector>

class GameMap;

struct Item {
    enum class Type {
        Growth,
        Poison
    };

    int y{};
    int x{};
    Type type{Type::Growth};
};

class ItemManager {
public:
    static constexpr int GrowthCount = 3;
    static constexpr int PoisonCount = 3;

    void regenerate(const GameMap& map,
                    const std::vector<std::pair<int, int>>& snakeBody);

    const std::vector<Item>& items() const;
    Item::Type consumeAt(int y, int x, bool& consumed);

private:
    bool isOccupied(int y,
                    int x,
                    const std::vector<std::pair<int, int>>& snakeBody) const;

    std::vector<Item> items_;
};

#endif
