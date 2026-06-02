// snake.h
// Snake 클래스의 몸통 좌표, 방향, 이동/충돌 인터페이스를 선언한다.

#ifndef SNAKE_H
#define SNAKE_H

#include <utility>
#include <vector>

class GameMap;

// 뱀의 몸 위치와 현재 이동 방향을 관리한다.
class Snake {
public:
    // 이동 방향은 snake.cpp의 행/열 변화량과 연결된다.
    enum class Direction {
        Up,
        Down,
        Left,
        Right
    };

    static constexpr int InitialLength = 3;
    static constexpr int MinimumLength = 3;

    // 주어진 맵 안에서 유효한 랜덤 위치에 뱀을 생성한다.
    explicit Snake(const GameMap& map);

    // 반대 방향 입력이면 false를 반환하고 Game이 게임 오버로 처리한다.
    bool changeDirection(Direction nextDirection);
    // 몸 상태를 바꾸지 않고 다음 머리 위치만 계산한다.
    std::pair<int, int> nextHead() const;
    // 뱀을 이동시킨다. grow가 true이면 꼬리를 유지해 길이가 늘어난다.
    void moveTo(const std::pair<int, int>& nextHead, bool grow);
    // 독 아이템을 먹었을 때 꼬리 한 칸을 제거한다.
    void shrink();

    // 충돌, 아이템 생성, 포탈 출구 계산에 사용하는 점유 확인 함수다.
    bool contains(int y, int x) const;
    bool hitsSelf(const std::pair<int, int>& pos) const;

    // 출력과 점수 계산에 사용하는 읽기 전용 접근 함수들이다.
    const std::vector<std::pair<int, int>>& body() const;
    int length() const;
    Direction direction() const;
    // 포탈 관리자가 포탈 출구 방향을 강제로 설정할 때 사용한다.
    void setDirection(Direction direction);

private:
    // 방향을 (dy, dx) 변화량으로 변환한다.
    static std::pair<int, int> delta(Direction direction);
    // 즉시 반대 방향 입력이 금지된다는 과제 규칙을 확인한다.
    bool isOpposite(Direction a, Direction b) const;

    // 몸통 배열의 첫 번째 원소는 항상 머리이고, 이후 원소들은 몸통과 꼬리다.
    std::vector<std::pair<int, int>> body_;
    Direction direction_{Direction::Right};
};

#endif
