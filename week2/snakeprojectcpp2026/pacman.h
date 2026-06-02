// pacman.h
// 적 캐릭터 Pacman의 위치, 방향, 이동 인터페이스를 선언한다.

#ifndef PACMAN_H
#define PACMAN_H

#include <utility>
#include <vector>

#include "snake.h"

class GameMap;
class ItemManager;
class PortalManager;

class Pacman {
public:
    static constexpr int MaxMovesInSameDirection = 10;

    explicit Pacman(const GameMap& map);

    void reset(const GameMap& map, const std::vector<std::pair<int, int>>& snakeBody);
    void update(const GameMap& map,
                const std::vector<std::pair<int, int>>& snakeBody,
                ItemManager& itemManager,
                const PortalManager& portalManager);

    const std::pair<int, int>& position() const;
    Snake::Direction direction() const;

private:
    static std::pair<int, int> delta(Snake::Direction direction);
    static Snake::Direction opposite(Snake::Direction direction);

    bool isBlocked(const std::pair<int, int>& pos,
                   const GameMap& map,
                   const std::vector<std::pair<int, int>>& snakeBody) const;
    std::vector<Snake::Direction> availableDirections(
        const GameMap& map,
        const std::vector<std::pair<int, int>>& snakeBody) const;

    std::pair<int, int> position_{1, 1};
    Snake::Direction direction_{Snake::Direction::Right};
    int movesInCurrentDirection_{0};
};

#endif
