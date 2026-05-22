#include "item.h"

#include <algorithm>
#include <cstdlib>
#include <stdexcept>

#include "map.h"

void ItemManager::regenerate(const GameMap& map,
                             const std::vector<std::pair<int, int>>& snakeBody)
{
    items_.clear();

    const int totalCount = GrowthCount + PoisonCount;
    int attempts = 0;

    while (static_cast<int>(items_.size()) < totalCount && attempts < 10000)
    {
        ++attempts;

        const int y = std::rand() % (GameMap::Size - 2) + 1;
        const int x = std::rand() % (GameMap::Size - 2) + 1;

        if (!map.isInside(y, x) || map.isWall(y, x) || map.isGate(y, x) ||
            isOccupied(y, x, snakeBody))
        {
            continue;
        }

        const bool duplicate = std::any_of(items_.begin(), items_.end(),
            [y, x](const Item& item) {
                return item.y == y && item.x == x;
            });

        if (duplicate)
        {
            continue;
        }

        const Item::Type type = static_cast<int>(items_.size()) < GrowthCount
            ? Item::Type::Growth
            : Item::Type::Poison;

        items_.push_back({y, x, type});
    }
}

const std::vector<Item>& ItemManager::items() const
{
    return items_;
}

Item::Type ItemManager::consumeAt(int y, int x, bool& consumed)
{
    for (auto it = items_.begin(); it != items_.end(); ++it)
    {
        if (it->y == y && it->x == x)
        {
            const Item::Type type = it->type;
            items_.erase(it);
            consumed = true;
            return type;
        }
    }

    consumed = false;
    return Item::Type::Growth;
}

bool ItemManager::isOccupied(int y,
                             int x,
                             const std::vector<std::pair<int, int>>& snakeBody) const
{
    return std::any_of(snakeBody.begin(), snakeBody.end(),
        [y, x](const std::pair<int, int>& pos) {
            return pos.first == y && pos.second == x;
        });
}
