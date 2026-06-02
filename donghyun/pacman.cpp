#include "pacman.h"

#include <algorithm>
#include <array>
#include <cstdlib>

#include "item.h"
#include "map.h"
#include "portal.h"

namespace {

Snake::Direction clockwise(Snake::Direction direction)
{
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

} // namespace

Pacman::Pacman(const GameMap& map)
{
    reset(map, {});
}

void Pacman::reset(const GameMap& map,
                   const std::vector<std::pair<int, int>>& snakeBody)
{
    int attempts = 0;

    while (attempts < 10000)
    {
        ++attempts;

        const int y = std::rand() % (GameMap::Size - 2) + 1;
        const int x = std::rand() % (GameMap::Size - 2) + 1;
        const std::pair<int, int> candidate{y, x};

        if (!isBlocked(candidate, map, snakeBody))
        {
            position_ = candidate;
            direction_ = static_cast<Snake::Direction>(std::rand() % 4);
            movesInCurrentDirection_ = 0;
            return;
        }
    }

    position_ = {1, 1};
    direction_ = Snake::Direction::Right;
    movesInCurrentDirection_ = 0;
}

void Pacman::update(const GameMap& map,
                    const std::vector<std::pair<int, int>>& snakeBody,
                    ItemManager& itemManager,
                    PortalManager& portalManager)
{
    std::vector<Snake::Direction> directions =
        availableDirections(map, snakeBody);

    if (directions.empty())
    {
        return;
    }

    const bool canKeepDirection =
        std::find(directions.begin(), directions.end(), direction_) !=
        directions.end();

    if (!canKeepDirection ||
        movesInCurrentDirection_ >= MaxMovesInSameDirection)
    {
        if (directions.size() > 1)
        {
            directions.erase(
                std::remove(directions.begin(), directions.end(), opposite(direction_)),
                directions.end());
        }

        if (movesInCurrentDirection_ >= MaxMovesInSameDirection &&
            directions.size() > 1)
        {
            directions.erase(
                std::remove(directions.begin(), directions.end(), direction_),
                directions.end());
        }

        direction_ = directions[std::rand() % directions.size()];
        movesInCurrentDirection_ = 0;
    }

    std::pair<int, int> next = {
        position_.first + delta(direction_).first,
        position_.second + delta(direction_).second
    };

    if (portalManager.isPortal(next.first, next.second))
    {
        const std::pair<int, int> exit = portalManager.getExitPortal(next);
        Snake::Direction exitDirection = direction_;

        if (!map.isInside(exit.first, exit.second))
        {
            return;
        }

        if (exit.first == 0)
        {
            exitDirection = Snake::Direction::Down;
            next = {exit.first + 1, exit.second};
        }
        else if (exit.first == GameMap::Size - 1)
        {
            exitDirection = Snake::Direction::Up;
            next = {exit.first - 1, exit.second};
        }
        else if (exit.second == 0)
        {
            exitDirection = Snake::Direction::Right;
            next = {exit.first, exit.second + 1};
        }
        else if (exit.second == GameMap::Size - 1)
        {
            exitDirection = Snake::Direction::Left;
            next = {exit.first, exit.second - 1};
        }
        else
        {
            const std::array<Snake::Direction, 4> priorities = {
                direction_,
                clockwise(direction_),
                counterClockwise(direction_),
                opposite(direction_)
            };

            bool foundExit = false;

            for (Snake::Direction candidateDirection : priorities)
            {
                const std::pair<int, int> d = delta(candidateDirection);
                const std::pair<int, int> candidate{
                    exit.first + d.first,
                    exit.second + d.second
                };

                if (!isBlocked(candidate, map, snakeBody))
                {
                    exitDirection = candidateDirection;
                    next = candidate;
                    foundExit = true;
                    break;
                }
            }

            if (!foundExit)
            {
                return;
            }
        }

        if (isBlocked(next, map, snakeBody))
        {
            return;
        }

        direction_ = exitDirection;
        movesInCurrentDirection_ = 0;
    }

    if (isBlocked(next, map, snakeBody))
    {
        return;
    }

    bool consumed = false;
    itemManager.consumeAt(next.first, next.second, consumed);
    position_ = next;
    ++movesInCurrentDirection_;
}

const std::pair<int, int>& Pacman::position() const
{
    return position_;
}

Snake::Direction Pacman::direction() const
{
    return direction_;
}

std::pair<int, int> Pacman::delta(Snake::Direction direction)
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

Snake::Direction Pacman::opposite(Snake::Direction direction)
{
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

bool Pacman::isBlocked(const std::pair<int, int>& pos,
                       const GameMap& map,
                       const std::vector<std::pair<int, int>>& snakeBody) const
{
    if (!map.isInside(pos.first, pos.second) ||
        map.isWall(pos.first, pos.second))
    {
        return true;
    }

    return std::any_of(snakeBody.begin(), snakeBody.end(),
        [&pos](const std::pair<int, int>& snakePos) {
            return snakePos == pos;
        });
}

std::vector<Snake::Direction> Pacman::availableDirections(
    const GameMap& map,
    const std::vector<std::pair<int, int>>& snakeBody) const
{
    const std::array<Snake::Direction, 4> candidates = {
        Snake::Direction::Up,
        Snake::Direction::Down,
        Snake::Direction::Left,
        Snake::Direction::Right
    };

    std::vector<Snake::Direction> directions;

    for (Snake::Direction direction : candidates)
    {
        const std::pair<int, int> d = delta(direction);
        const std::pair<int, int> next{
            position_.first + d.first,
            position_.second + d.second
        };

        if (map.isInside(next.first, next.second) &&
            (map.isGate(next.first, next.second) ||
             !isBlocked(next, map, snakeBody)))
        {
            directions.push_back(direction);
        }
    }

    return directions;
}
