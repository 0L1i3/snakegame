#ifndef MAP_H
#define MAP_H

#include <ncurses.h>
#include <utility>
#include <vector>

struct Item;

class GameMap {
public:
    static constexpr int Size = 50;
    static constexpr int DrawWidth = Size * 2;

    enum class Cell {
        Empty,
        Wall,
        ImmuneWall,
        SnakeHead,
        SnakeBody,
        GrowthItem,
        PoisonItem,
        Gate
    };

    GameMap();

    bool isWall(int y, int x) const;
    bool isImmuneWall(int y, int x) const;
    bool isInside(int y, int x) const;
    bool isGate(int y, int x) const;
    void clearGates();
    void setGate(const std::pair<int, int>& pos);

    std::vector<std::pair<int, int>> getWallPositionsForGate() const;

    void draw(WINDOW* window,
              const std::vector<std::pair<int, int>>& snakeBody,
              const std::vector<Item>& items,
              int length) const;

private:
    Cell cells_[Size][Size];
};

void setColor();

#endif
