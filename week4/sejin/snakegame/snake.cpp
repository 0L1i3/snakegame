#include "snake.h"

#include <cstdlib>

#include "map.h"

Snake::Snake(const GameMap& map)
{
    while (true)
    {
        const int y = std::rand() % (GameMap::Size - 10) + 5;
        const int x = std::rand() % (GameMap::Size - 10) + 5;
        const Direction randomDirection = static_cast<Direction>(std::rand() % 4);
        const std::pair<int, int> d = delta(randomDirection);

        body_.clear();
        bool possible = true;

        for (int i = 0; i < InitialLength; ++i)
        {
            const int bodyY = y - d.first * i;
            const int bodyX = x - d.second * i;

            if (!map.isInside(bodyY, bodyX) || map.isWall(bodyY, bodyX))
            {
                possible = false;
                break;
            }

            body_.push_back({bodyY, bodyX});
        }

        if (possible)
        {
            direction_ = randomDirection;
            return;
        }
    }
}

void Snake::changeDirection(Direction nextDirection)
{
    if (!isOpposite(direction_, nextDirection))
    {
        direction_ = nextDirection;
    }
}

std::pair<int, int> Snake::nextHead() const
{
    const std::pair<int, int> d = delta(direction_);
    return {body_.front().first + d.first, body_.front().second + d.second};
}

void Snake::moveTo(const std::pair<int, int>& nextHead, bool grow)
{
    body_.insert(body_.begin(), nextHead);

    if (!grow)
    {
        body_.pop_back();
    }
}

void Snake::shrink()
{
    if (!body_.empty())
    {
        body_.pop_back();
    }
}

bool Snake::contains(int y, int x) const
{
    for (const std::pair<int, int>& pos : body_)
    {
        if (pos.first == y && pos.second == x)
        {
            return true;
        }
    }

    return false;
}

bool Snake::hitsSelf(const std::pair<int, int>& pos) const
{
    for (std::size_t i = 1; i < body_.size(); ++i)
    {
        if (body_[i] == pos)
        {
            return true;
        }
    }

    return false;
}

const std::vector<std::pair<int, int>>& Snake::body() const
{
    return body_;
}

int Snake::length() const
{
    return static_cast<int>(body_.size());
}

Snake::Direction Snake::direction() const
{
    return direction_;
}

void Snake::setDirection(Direction direction)
{
    direction_ = direction;
}

std::pair<int, int> Snake::delta(Direction direction)
{
    switch (direction)
    {
        case Direction::Up:
            return {-1, 0};
        case Direction::Down:
            return {1, 0};
        case Direction::Left:
            return {0, -1};
        case Direction::Right:
            return {0, 1};
    }

    return {0, 1};
}

bool Snake::isOpposite(Direction a, Direction b) const
{
    return (a == Direction::Up && b == Direction::Down) ||
           (a == Direction::Down && b == Direction::Up) ||
           (a == Direction::Left && b == Direction::Right) ||
           (a == Direction::Right && b == Direction::Left);
}
