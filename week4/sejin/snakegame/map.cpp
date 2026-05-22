#include "map.h"

#include <algorithm>

#include "item.h"

GameMap::GameMap()
{
    for (int y = 0; y < Size; ++y)
    {
        for (int x = 0; x < Size; ++x)
        {
            const bool corner = (y == 0 || y == Size - 1) &&
                                (x == 0 || x == Size - 1);
            const bool edge = y == 0 || y == Size - 1 || x == 0 || x == Size - 1;

            if (corner)
            {
                cells_[y][x] = Cell::ImmuneWall;
            }
            else if (edge)
            {
                cells_[y][x] = Cell::Wall;
            }
            else
            {
                cells_[y][x] = Cell::Empty;
            }
        }
    }
}

bool GameMap::isWall(int y, int x) const
{
    if (!isInside(y, x))
    {
        return true;
    }

    return cells_[y][x] == Cell::Wall || cells_[y][x] == Cell::ImmuneWall;
}

bool GameMap::isImmuneWall(int y, int x) const
{
    return isInside(y, x) && cells_[y][x] == Cell::ImmuneWall;
}

bool GameMap::isInside(int y, int x) const
{
    return y >= 0 && y < Size && x >= 0 && x < Size;
}

bool GameMap::isGate(int y, int x) const
{
    return isInside(y, x) && cells_[y][x] == Cell::Gate;
}

void GameMap::clearGates()
{
    for (int y = 0; y < Size; ++y)
    {
        for (int x = 0; x < Size; ++x)
        {
            if (cells_[y][x] == Cell::Gate)
            {
                cells_[y][x] = Cell::Wall;
            }
        }
    }
}

void GameMap::setGate(const std::pair<int, int>& pos)
{
    if (isInside(pos.first, pos.second) && !isImmuneWall(pos.first, pos.second))
    {
        cells_[pos.first][pos.second] = Cell::Gate;
    }
}

std::vector<std::pair<int, int>> GameMap::getWallPositionsForGate() const
{
    std::vector<std::pair<int, int>> positions;

    for (int y = 0; y < Size; ++y)
    {
        for (int x = 0; x < Size; ++x)
        {
            if (cells_[y][x] == Cell::Wall)
            {
                positions.push_back({y, x});
            }
        }
    }

    return positions;
}

void GameMap::draw(WINDOW* window,
                   const std::vector<std::pair<int, int>>& snakeBody,
                   const std::vector<Item>& items,
                   int length) const
{
    Cell board[Size][Size];

    for (int y = 0; y < Size; ++y)
    {
        for (int x = 0; x < Size; ++x)
        {
            board[y][x] = cells_[y][x];
        }
    }

    for (const Item& item : items)
    {
        board[item.y][item.x] = item.type == Item::Type::Growth
            ? Cell::GrowthItem
            : Cell::PoisonItem;
    }

    if (!snakeBody.empty())
    {
        board[snakeBody.front().first][snakeBody.front().second] = Cell::SnakeHead;

        for (std::size_t i = 1; i < snakeBody.size(); ++i)
        {
            board[snakeBody[i].first][snakeBody[i].second] = Cell::SnakeBody;
        }
    }

    wclear(window);

    for (int y = 0; y < Size; ++y)
    {
        for (int x = 0; x < Size; ++x)
        {
            switch (board[y][x])
            {
                case Cell::Empty:
                    mvwprintw(window, y, x * 2, "  ");
                    break;
                case Cell::Wall:
                    mvwprintw(window, y, x * 2, "[]");
                    break;
                case Cell::ImmuneWall:
                    mvwprintw(window, y, x * 2, "##");
                    break;
                case Cell::SnakeHead:
                    wattron(window, COLOR_PAIR(1));
                    mvwprintw(window, y, x * 2, "@@");
                    wattroff(window, COLOR_PAIR(1));
                    break;
                case Cell::SnakeBody:
                    wattron(window, COLOR_PAIR(2));
                    mvwprintw(window, y, x * 2, "oo");
                    wattroff(window, COLOR_PAIR(2));
                    break;
                case Cell::GrowthItem:
                    wattron(window, COLOR_PAIR(3));
                    mvwprintw(window, y, x * 2, "G+");
                    wattroff(window, COLOR_PAIR(3));
                    break;
                case Cell::PoisonItem:
                    wattron(window, COLOR_PAIR(4));
                    mvwprintw(window, y, x * 2, "P-");
                    wattroff(window, COLOR_PAIR(4));
                    break;
                case Cell::Gate:
                    wattron(window, COLOR_PAIR(5));
                    mvwprintw(window, y, x * 2, "<>");
                    wattroff(window, COLOR_PAIR(5));
                    break;
            }
        }
    }

    mvwprintw(window, Size + 1, 0, "Length : %d", length);
    mvwprintw(window, Size + 2, 0, "Move: Arrow keys / WASD    Quit: q");
    wrefresh(window);
}

void setColor()
{
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
}
