#include "map.h"

#include <algorithm>

#include "item.h"

GameMap::GameMap()
{
    // 기본 생성 시 1스테이지 맵을 만든다.
    loadStage(1);
}

void GameMap::loadStage(int stage)
{
    // 먼저 공통 테두리를 만든다. 모서리는 Immune Wall, 가장자리는 Wall, 내부는 Empty다.
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

    switch (stage)
    {
        case 2:
            // 2스테이지는 세로 벽 두 줄과 가운데 연결 벽을 사용한다.
            for (int y = 4; y < Size - 4; ++y)
            {
                if (y != Size / 2)
                {
                    cells_[y][6] = Cell::Wall;
                    cells_[y][14] = Cell::Wall;
                }
            }
            for (int x = 7; x < 14; ++x)
            {
                cells_[Size / 2][x] = Cell::Wall;
            }
            break;
        case 3:
            // 3스테이지는 중앙 주변에 통로가 있는 사각형 장애물 구조다.
            for (int x = 4; x < Size - 4; ++x)
            {
                if (x < 8 || x > 12)
                {
                    cells_[5][x] = Cell::Wall;
                    cells_[15][x] = Cell::Wall;
                }
            }
            for (int y = 5; y <= 15; ++y)
            {
                if (y != 10)
                {
                    cells_[y][5] = Cell::Wall;
                    cells_[y][15] = Cell::Wall;
                }
            }
            cells_[5][5] = Cell::ImmuneWall;
            cells_[5][15] = Cell::ImmuneWall;
            cells_[15][5] = Cell::ImmuneWall;
            cells_[15][15] = Cell::ImmuneWall;
            break;
        case 4:
            // 4스테이지는 대각선 장애물과 중앙 Immune Wall을 사용하는 더 어려운 구조다.
            for (int i = 4; i < Size - 4; ++i)
            {
                if (i != Size / 2)
                {
                    cells_[i][i] = Cell::Wall;
                    cells_[i][Size - 1 - i] = Cell::Wall;
                }
            }
            for (int x = 7; x <= 13; ++x)
            {
                if (x != Size / 2)
                {
                    cells_[Size / 2][x] = Cell::Wall;
                }
            }
            cells_[Size / 2][Size / 2] = Cell::ImmuneWall;
            break;
        case 1:
        default:
            // 1스테이지는 중앙 통로가 있는 가로 벽 두 줄로 시작한다.
            for (int x = 5; x < Size - 5; ++x)
            {
                if (x != Size / 2)
                {
                    cells_[7][x] = Cell::Wall;
                    cells_[13][x] = Cell::Wall;
                }
            }
            break;
    }
}

bool GameMap::isWall(int y, int x) const
{
    if (!isInside(y, x))
    {
        // 맵 밖은 충돌 판정을 위해 벽처럼 취급한다.
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
    // 게이트는 항상 일반 벽 위에 생성되므로 제거할 때 벽으로 복구한다.
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
    // 일반 Wall만 Gate가 될 수 있고 Immune Wall은 보호된다.
    if (isInside(pos.first, pos.second) &&
        cells_[pos.first][pos.second] == Cell::Wall)
    {
        cells_[pos.first][pos.second] = Cell::Gate;
    }
}

std::vector<std::pair<int, int>> GameMap::getWallPositionsForGate() const
{
    std::vector<std::pair<int, int>> positions;

    // 포탈 관리자가 이 목록을 섞어서 랜덤 게이트 한 쌍을 고른다.
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
                   int length,
                   const std::pair<int, int>& pacmanPosition) const
{
    static_cast<void>(length);
    Cell board[Size][Size];

    // 먼저 고정 맵 셀을 복사한다.
    for (int y = 0; y < Size; ++y)
    {
        for (int x = 0; x < Size; ++x)
        {
            board[y][x] = cells_[y][x];
        }
    }

    // 아이템은 동적 오브젝트이므로 임시 보드 위에 덮어 그린다.
    for (const Item& item : items)
    {
        board[item.y][item.x] = item.type == Item::Type::Growth
            ? Cell::GrowthItem
            : Cell::PoisonItem;
    }

    if (!snakeBody.empty())
    {
        // 뱀은 가장 마지막에 그려서 화면에서 우선 보이게 한다.
        board[snakeBody.front().first][snakeBody.front().second] = Cell::SnakeHead;

        for (std::size_t i = 1; i < snakeBody.size(); ++i)
        {
            board[snakeBody[i].first][snakeBody[i].second] = Cell::SnakeBody;
        }
    }

    if (isInside(pacmanPosition.first, pacmanPosition.second))
    {
        board[pacmanPosition.first][pacmanPosition.second] = Cell::Pacman;
    }

    wclear(window);

    // 맵 비율을 맞추기 위해 모든 셀을 두 글자로 출력한다.
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
                case Cell::Pacman:
                    wattron(window, COLOR_PAIR(6));
                    mvwprintw(window, y, x * 2, "CC");
                    wattroff(window, COLOR_PAIR(6));
                    break;
            }
        }
    }

}

void setColor()
{
    // 출력 함수에서 번호로 참조하는 색상 쌍을 설정한다.
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_BLUE, COLOR_BLACK);
}
