// portal.h
// Gate 위치와 포탈 출구 계산을 담당하는 PortalManager를 선언한다.

#ifndef PORTAL_H
#define PORTAL_H

#include <utility>

#include "snake.h"

class GameMap;

// 게이트와 포탈 한 쌍을 관리하고 유효한 출구 위치를 계산한다.
class PortalManager {
public:
    // 일반 Wall 위치에 새로운 포탈 한 쌍을 생성한다.
    void generate(GameMap& map);
    // 포탈을 제거하고 해당 셀을 Wall로 되돌린다.
    void clear(GameMap& map);
    bool hasPortal() const;
    bool isPortal(int y, int x) const;

    // 입구 포탈과 짝이 되는 반대쪽 포탈 위치를 반환한다.
    std::pair<int, int> getExitPortal(const std::pair<int, int>& entrance) const;

    // 과제의 포탈 진출 방향 우선순위 규칙을 적용한다.
    std::pair<int, int> calculateExitPosition(
        const std::pair<int, int>& entrance,
        Snake::Direction currentDirection,
        Snake::Direction& newDirection,
        const GameMap& map,
        const Snake& snake) const;

    // 뱀이 포탈을 통과 중일 때 시간 기반 재생성을 막기 위한 상태다.
    bool isSnakeUsingPortal() const;
    void setSnakeUsingPortal(bool usingPortal);

private:
    // 현재 활성 포탈 한 쌍이다. {-1, -1}은 유효한 포탈이 없다는 뜻이다.
    std::pair<int, int> portalA_{-1, -1};
    std::pair<int, int> portalB_{-1, -1};
    bool active_{false};
    bool snakeUsingPortal_{false};
};

#endif
