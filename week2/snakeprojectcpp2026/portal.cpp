// portal.cpp
// 벽 위에 생성되는 Gate 한 쌍과 포탈 진입 후 출구 방향 계산 규칙을 구현한다.

#include "portal.h"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>

#include "map.h"

namespace {

// 방향 계산 보조 함수들은 PortalManager의 공개 API 밖에 둔다.
std::pair<int, int> delta(Snake::Direction direction)
{
    switch (direction)
    {
        case Snake::Direction::Up:
            return {-1, 0};
        case Snake::Direction::Down:
            return {1, 0};
        case Snake::Direction::Left:
            return {0, -1};
        case Snake::Direction::Right:
            return {0, 1};
    }

    return {0, 1};
}

Snake::Direction clockwise(Snake::Direction direction)
{
    // 포탈 진출 우선순위에서 직진 다음 방향을 구할 때 사용한다.
    switch (direction)
    {
        case Snake::Direction::Up:
            return Snake::Direction::Right;
        case Snake::Direction::Right:
            return Snake::Direction::Down;
        case Snake::Direction::Down:
            return Snake::Direction::Left;
        case Snake::Direction::Left:
            return Snake::Direction::Up;
    }

    return Snake::Direction::Right;
}

Snake::Direction counterClockwise(Snake::Direction direction)
{
    // 포탈 진출 우선순위에서 시계 방향 다음의 반시계 방향을 구할 때 사용한다.
    switch (direction)
    {
        case Snake::Direction::Up:
            return Snake::Direction::Left;
        case Snake::Direction::Left:
            return Snake::Direction::Down;
        case Snake::Direction::Down:
            return Snake::Direction::Right;
        case Snake::Direction::Right:
            return Snake::Direction::Up;
    }

    return Snake::Direction::Left;
}

Snake::Direction opposite(Snake::Direction direction)
{
    // 내부 Wall 포탈에서 마지막으로 시도하는 반대 방향을 구한다.
    switch (direction)
    {
        case Snake::Direction::Up:
            return Snake::Direction::Down;
        case Snake::Direction::Down:
            return Snake::Direction::Up;
        case Snake::Direction::Left:
            return Snake::Direction::Right;
        case Snake::Direction::Right:
            return Snake::Direction::Left;
    }

    return Snake::Direction::Left;
}

std::pair<int, int> moveFrom(const std::pair<int, int>& pos,
                             Snake::Direction direction)
{
    // 주어진 방향으로 한 칸 이동한 이웃 셀을 반환한다.
    const std::pair<int, int> d = delta(direction);
    return {pos.first + d.first, pos.second + d.second};
}

bool isBlocked(const std::pair<int, int>& pos,
               const GameMap& map,
               const Snake& snake)
{
    // 포탈 출구는 Wall, Immune Wall, 뱀 몸 위에 위치할 수 없다.
    return !map.isInside(pos.first, pos.second) ||
           map.isWall(pos.first, pos.second) ||
           snake.contains(pos.first, pos.second);
}

} // 익명 네임스페이스

void PortalManager::generate(GameMap& map)
{
    if (snakeUsingPortal_)
    {
        // 과제 규칙상 뱀이 Gate를 통과 중이면 Gate가 사라지면 안 된다.
        return;
    }

    clear(map);

    std::vector<std::pair<int, int>> wallPositions = map.getWallPositionsForGate();

    if (wallPositions.size() < 2)
    {
        // 포탈 한 쌍을 만들려면 서로 다른 Wall 위치가 두 개 필요하다.
        return;
    }

    // 후보 Wall 목록을 섞고 앞의 두 위치를 포탈 한 쌍으로 사용한다.
    static std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
    std::shuffle(wallPositions.begin(), wallPositions.end(), generator);

    portalA_ = wallPositions[0];
    portalB_ = wallPositions[1];
    active_ = true;

    map.setGate(portalA_);
    map.setGate(portalB_);
}

void PortalManager::clear(GameMap& map)
{
    if (snakeUsingPortal_)
    {
        // 뱀이 완전히 빠져나올 때까지 두 Gate를 모두 유지한다.
        return;
    }

    map.clearGates();
    portalA_ = {-1, -1};
    portalB_ = {-1, -1};
    active_ = false;
}

bool PortalManager::hasPortal() const
{
    return active_;
}

bool PortalManager::isPortal(int y, int x) const
{
    // 한 번에 한 쌍만 존재하므로 두 포탈 위치를 모두 검사한다.
    return active_ &&
           ((portalA_.first == y && portalA_.second == x) ||
            (portalB_.first == y && portalB_.second == x));
}

std::pair<int, int> PortalManager::getExitPortal(
    const std::pair<int, int>& entrance) const
{
    if (!active_)
    {
        return {-1, -1};
    }

    // 입구가 A이면 B로, 그렇지 않으면 A로 나온다.
    return entrance == portalA_ ? portalB_ : portalA_;
}

std::pair<int, int> PortalManager::calculateExitPosition(
    const std::pair<int, int>& entrance,
    Snake::Direction currentDirection,
    Snake::Direction& newDirection,
    const GameMap& map,
    const Snake& snake) const
{
    const std::pair<int, int> exit = getExitPortal(entrance);

    if (!map.isInside(exit.first, exit.second))
    {
        return {-1, -1};
    }

    if (exit.first == 0)
    {
        // 가장자리 Gate는 뱀이 맵 안쪽으로 나오도록 방향을 강제한다.
        newDirection = Snake::Direction::Down;
        return {exit.first + 1, exit.second};
    }
    if (exit.first == GameMap::Size - 1)
    {
        newDirection = Snake::Direction::Up;
        return {exit.first - 1, exit.second};
    }
    if (exit.second == 0)
    {
        newDirection = Snake::Direction::Right;
        return {exit.first, exit.second + 1};
    }
    if (exit.second == GameMap::Size - 1)
    {
        newDirection = Snake::Direction::Left;
        return {exit.first, exit.second - 1};
    }

    const std::array<Snake::Direction, 4> priorities = {
        currentDirection,
        clockwise(currentDirection),
        counterClockwise(currentDirection),
        opposite(currentDirection)
    };

    // 내부 Wall Gate는 직진, 시계 방향, 반시계 방향, 반대 방향 순서로 검사한다.
    for (Snake::Direction direction : priorities)
    {
        const std::pair<int, int> candidate = moveFrom(exit, direction);

        if (!isBlocked(candidate, map, snake))
        {
            newDirection = direction;
            return candidate;
        }
    }

    return {-1, -1};
}

bool PortalManager::isSnakeUsingPortal() const
{
    return snakeUsingPortal_;
}

void PortalManager::setSnakeUsingPortal(bool usingPortal)
{
    snakeUsingPortal_ = usingPortal;
}
