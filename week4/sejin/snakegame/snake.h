#ifndef SNAKE_H
#define SNAKE_H

#include <utility>
#include <vector>

class GameMap;

class Snake {
public:
    enum class Direction {
        Up,
        Down,
        Left,
        Right
    };

    static constexpr int InitialLength = 3;
    static constexpr int MinimumLength = 3;

    explicit Snake(const GameMap& map);

    void changeDirection(Direction nextDirection);
    std::pair<int, int> nextHead() const;
    void moveTo(const std::pair<int, int>& nextHead, bool grow);
    void shrink();

    bool contains(int y, int x) const;
    bool hitsSelf(const std::pair<int, int>& pos) const;

    const std::vector<std::pair<int, int>>& body() const;
    int length() const;
    Direction direction() const;
    void setDirection(Direction direction);

private:
    static std::pair<int, int> delta(Direction direction);
    bool isOpposite(Direction a, Direction b) const;

    std::vector<std::pair<int, int>> body_;
    Direction direction_{Direction::Right};
};

#endif
