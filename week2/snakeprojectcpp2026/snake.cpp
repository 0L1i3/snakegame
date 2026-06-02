// snake.cpp
// 뱀의 초기 배치, 방향 전환, 이동, 충돌 판정을 구현한다.

#include "snake.h"

#include <cstdlib>

#include "map.h"

Snake::Snake(const GameMap& map)
{
    // 초기 몸통이 맵 안에 들어갈 때까지 머리 위치와 방향을 랜덤으로 시도한다.
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
            // 뱀이 d 방향을 바라보도록 머리에서 반대 방향으로 몸통을 만든다.
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

bool Snake::changeDirection(Direction nextDirection)
{
    // 거짓을 반환하면 게임 객체가 반대 방향 입력 게임 오버 규칙을 적용한다.
    if (isOpposite(direction_, nextDirection))
    {
        return false;
    }

    direction_ = nextDirection;
    return true;
}

std::pair<int, int> Snake::nextHead() const
{
    const std::pair<int, int> d = delta(direction_);
    return {body_.front().first + d.first, body_.front().second + d.second};
}

void Snake::moveTo(const std::pair<int, int>& nextHead, bool grow)
{
    // 새 머리를 먼저 추가하고, 성장 중이 아니면 꼬리를 제거해 길이를 유지한다.
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
    // 0번은 현재 머리이므로 자기 몸 충돌 검사에서 제외한다.
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
    // 보드 좌표는 y가 먼저이므로 Up은 y 감소, Left는 x 감소다.
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
