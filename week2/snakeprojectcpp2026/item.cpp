// item.cpp
// 성장, 독, 무적 아이템의 생성, 수명, 소비 규칙을 관리한다.

#include "item.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>

#include "map.h"

void ItemManager::initStage(const GameMap& map,
                            const std::vector<std::pair<int, int>>& snakeBody)
{
    // 스테이지 시작: 모든 아이템과 무적 타이머를 완전히 초기화한다.
    items_.clear();
    lastInvincibleSpawn_ = std::chrono::steady_clock::now();

    while (countRegularItems() < MaxItemCount)
    {
        if (!createRandomItem(map, snakeBody))
        {
            break;
        }
    }
}

void ItemManager::regenerate(const GameMap& map,
                             const std::vector<std::pair<int, int>>& snakeBody)
{
    // 10초 주기 재생성: Growth/Poison만 교체하고 무적 아이템과 타이머는 건드리지 않는다.
    items_.erase(
        std::remove_if(items_.begin(), items_.end(),
            [](const Item& item) {
                return item.type != Item::Type::Invincible;
            }),
        items_.end());

    while (countRegularItems() < MaxItemCount)
    {
        if (!createRandomItem(map, snakeBody))
        {
            break;
        }
    }
}

void ItemManager::update(const GameMap& map,
                         const std::vector<std::pair<int, int>>& snakeBody)
{
    using namespace std::chrono;
    const auto now = steady_clock::now();

    // Growth/Poison 아이템만 수명 만료 처리한다.
    items_.erase(
        std::remove_if(items_.begin(), items_.end(),
            [now](const Item& item) {
                if (item.type == Item::Type::Invincible)
                {
                    return false;
                }
                return duration_cast<seconds>(now - item.createdAt).count() >=
                       LifetimeSeconds;
            }),
        items_.end());

    // 빈 Growth/Poison 슬롯을 채운다.
    while (countRegularItems() < MaxItemCount)
    {
        if (!createRandomItem(map, snakeBody))
        {
            break;
        }
    }

    // 무적 아이템 수명 만료 처리.
    const bool hadInvincible = std::any_of(items_.begin(), items_.end(),
        [](const Item& i) { return i.type == Item::Type::Invincible; });

    if (hadInvincible)
    {
        items_.erase(
            std::remove_if(items_.begin(), items_.end(),
                [now](const Item& item) {
                    if (item.type != Item::Type::Invincible)
                    {
                        return false;
                    }
                    return duration_cast<seconds>(now - item.createdAt).count() >=
                           InvincibleLifetimeSeconds;
                }),
            items_.end());

        const bool stillHas = std::any_of(items_.begin(), items_.end(),
            [](const Item& i) { return i.type == Item::Type::Invincible; });

        // 만료로 사라졌으면 30초 타이머를 리셋한다.
        if (!stillHas)
        {
            lastInvincibleSpawn_ = now;
        }
    }
    else
    {
        // 무적 아이템이 없고 30초가 지나면 새로 생성한다.
        if (duration_cast<seconds>(now - lastInvincibleSpawn_).count() >=
            InvincibleSpawnSeconds)
        {
            spawnInvincibleItem(map, snakeBody);
        }
    }
}

const std::vector<Item>& ItemManager::items() const
{
    return items_;
}

Item::Type ItemManager::consumeAt(int y, int x, bool& consumed)
{
    // 소비된 아이템은 즉시 제거해 중복으로 먹을 수 없게 한다.
    for (auto it = items_.begin(); it != items_.end(); ++it)
    {
        if (it->y == y && it->x == x)
        {
            const Item::Type type = it->type;
            items_.erase(it);
            consumed = true;
            // 무적 아이템이 소비되면 30초 타이머를 즉시 리셋한다.
            if (type == Item::Type::Invincible)
            {
                lastInvincibleSpawn_ = std::chrono::steady_clock::now();
            }
            return type;
        }
    }

    consumed = false;
    return Item::Type::Growth;
}

bool ItemManager::createRandomItem(
    const GameMap& map,
    const std::vector<std::pair<int, int>>& snakeBody)
{
    int attempts = 0;

    // 보드가 복잡하면 랜덤 배치가 실패할 수 있으므로 시도 횟수를 제한한다.
    while (attempts < 10000)
    {
        ++attempts;

        const int y = std::rand() % (GameMap::Size - 2) + 1;
        const int x = std::rand() % (GameMap::Size - 2) + 1;

        // 아이템은 벽, Gate, 뱀, 다른 아이템과 겹치면 안 된다.
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

        // 갱신 함수에서 수명을 계산할 수 있도록 생성 시각을 저장한다.
        items_.push_back({y, x, nextItemType(), std::chrono::steady_clock::now()});
        return true;
    }

    return false;
}

bool ItemManager::spawnInvincibleItem(
    const GameMap& map,
    const std::vector<std::pair<int, int>>& snakeBody)
{
    int attempts = 0;

    while (attempts < 10000)
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

        items_.push_back({y, x, Item::Type::Invincible, std::chrono::steady_clock::now()});
        return true;
    }

    return false;
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

Item::Type ItemManager::nextItemType() const
{
    // 설정된 Growth 개수를 채울 때까지 Growth 아이템을 우선 생성한다.
    const int growthItems = static_cast<int>(
        std::count_if(items_.begin(), items_.end(),
            [](const Item& item) {
                return item.type == Item::Type::Growth;
            }));

    if (growthItems < GrowthCount)
    {
        return Item::Type::Growth;
    }

    return Item::Type::Poison;
}

int ItemManager::countRegularItems() const
{
    return static_cast<int>(
        std::count_if(items_.begin(), items_.end(),
            [](const Item& item) {
                return item.type != Item::Type::Invincible;
            }));
}
