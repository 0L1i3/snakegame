// item.h
// 보드 위 아이템 데이터와 아이템 생성/갱신을 담당하는 ItemManager를 선언한다.

#ifndef ITEM_H
#define ITEM_H

#include <chrono>
#include <utility>
#include <vector>

class GameMap;

// 보드 위의 Growth 또는 Poison 아이템 하나를 표현한다.
struct Item {
    // 성장 아이템은 길이를 늘리고 독 아이템은 길이를 줄인다.
    enum class Type {
        Growth,
        Poison,
        Invincible
    };

    int y{};
    int x{};
    Type type{Type::Growth};
    // 아이템 수명이 끝났는지 판단하기 위해 생성 시각을 저장한다.
    std::chrono::steady_clock::time_point createdAt{};
};

// 현재 등장한 아이템을 관리하고 총 3개 제한을 지킨다.
class ItemManager {
public:
    // 현재 정책은 Growth 2개, Poison 1개를 생성한다.
    static constexpr int GrowthCount = 2;
    static constexpr int PoisonCount = 1;
    static constexpr int MaxItemCount = GrowthCount + PoisonCount;
    static constexpr int LifetimeSeconds = 10;

    static constexpr int InvincibleLifetimeSeconds = 10;
    static constexpr int InvincibleSpawnSeconds = 30;
    static constexpr int InvincibleEffectSeconds = 5;

    // 스테이지 시작 시 무적 타이머를 포함한 전체 상태를 초기화한다.
    void initStage(const GameMap& map,
                   const std::vector<std::pair<int, int>>& snakeBody);
    // 10초 주기 재생성: Growth/Poison만 갱신하고 무적 아이템은 그대로 둔다.
    void regenerate(const GameMap& map,
                    const std::vector<std::pair<int, int>>& snakeBody);
    // 만료된 아이템을 제거하고 빈 슬롯을 채운다.
    void update(const GameMap& map,
                const std::vector<std::pair<int, int>>& snakeBody);

    const std::vector<Item>& items() const;
    // 특정 위치의 아이템을 제거하고 실제 소비 여부를 알려준다.
    Item::Type consumeAt(int y, int x, bool& consumed);

private:
    // 유효한 랜덤 위치에 아이템 하나를 배치한다.
    bool createRandomItem(const GameMap& map,
                          const std::vector<std::pair<int, int>>& snakeBody);
    // 30초 주기로 무적 아이템 하나를 랜덤 위치에 배치한다.
    bool spawnInvincibleItem(const GameMap& map,
                             const std::vector<std::pair<int, int>>& snakeBody);
    // 아이템이 뱀 몸 위에 생성되지 않도록 확인한다.
    bool isOccupied(int y,
                    int x,
                    const std::vector<std::pair<int, int>>& snakeBody) const;
    // 성장 아이템 개수를 먼저 채운 뒤 독 아이템을 생성하도록 타입을 결정한다.
    Item::Type nextItemType() const;
    // Growth/Poison 아이템만 세어서 MaxItemCount 제한에 사용한다.
    int countRegularItems() const;

    std::vector<Item> items_;
    // 무적 아이템은 30초 주기로 별도 관리한다.
    std::chrono::steady_clock::time_point lastInvincibleSpawn_{};
};

#endif
