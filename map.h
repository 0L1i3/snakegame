#ifndef MAP_H
#define MAP_H

#include <ncurses.h>
#include <utility>
#include <vector>

struct Item;

// 현재 스테이지의 고정 맵 셀을 관리하고 전체 맵을 출력한다.
class GameMap {
public:
    // 프로젝트는 21x21 맵을 사용하며, 한 칸을 두 글자로 그리므로 출력 너비는 두 배다.
    static constexpr int Size = 21;
    static constexpr int DrawWidth = Size * 2;

    // 과제 기준 셀 값이다. 0은 빈 칸, 7은 Gate에 대응된다.
    enum class Cell {
        Empty,
        Wall,
        ImmuneWall,
        SnakeHead,
        SnakeBody,
        GrowthItem,
        PoisonItem,
        Gate,
        Pacman
    };

    GameMap();

    // 스테이지 번호에 맞는 맵 구조를 다시 만든다.
    void loadStage(int stage);

    // 뱀, 아이템 관리자, 포탈 관리자가 사용하는 맵 상태 확인 함수들이다.
    bool isWall(int y, int x) const;
    bool isImmuneWall(int y, int x) const;
    bool isInside(int y, int x) const;
    bool isGate(int y, int x) const;

    // 게이트가 사라질 때 게이트 셀을 일반 벽으로 되돌린다.
    void clearGates();
    void setGate(const std::pair<int, int>& pos);

    // 면역 벽을 제외한 일반 벽 위치만 반환한다.
    std::vector<std::pair<int, int>> getWallPositionsForGate() const;

    // 맵, 뱀, 아이템을 임시 보드에 합쳐서 출력한다.
    void draw(WINDOW* window,
              const std::vector<std::pair<int, int>>& snakeBody,
              const std::vector<Item>& items,
              int length,
              const std::pair<int, int>& pacmanPosition) const;

private:
    // 고정 맵 셀만 저장한다. 뱀과 아이템은 draw()에서 임시로 합성한다.
    Cell cells_[Size][Size];
};

// 보드 출력에 사용하는 ncurses 색상 쌍을 초기화한다.
void setColor();

#endif
