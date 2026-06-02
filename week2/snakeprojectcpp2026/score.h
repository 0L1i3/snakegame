// score.h
// 게임 점수와 미션 판정용 카운터를 관리하는 Score 클래스를 선언한다.

#ifndef SCORE_H
#define SCORE_H

#include <chrono>

#include <ncurses.h>

// 점수판에 표시되는 모든 값을 관리한다.
class Score {
public:
    // 스테이지별 점수 카운터를 초기화하고 타이머를 다시 시작한다.
    void reset();

    // 성공적으로 이동한 뒤 현재 길이와 최대 길이를 갱신한다.
    void updateLength(int currentLength);
    // 아이템과 Gate 카운터는 미션 달성 판정에 사용된다.
    void addGrowth();
    void addPoison();
    void addGate();
    // n점을 누적 점수에 더한다 (스테이지 리셋 시에도 유지).
    void addPoints(int n);
    // 진행 시간 측정을 위한 함수들이다.
    void startTimer();
    int elapsedSeconds() const;

    // 출력과 미션 판정에 사용하는 읽기 함수들이다.
    int currentLength() const;
    int maxLength() const;
    int growthCount() const;
    int poisonCount() const;
    int gateCount() const;
    int totalScore() const;

    // 호출자가 지정한 위치에 Score Board를 출력한다.
    void draw(WINDOW* window, int startY, int startX, int highScore) const;

private:
    int currentLength_{};
    int maxLength_{};
    int growthCount_{};
    int poisonCount_{};
    int gateCount_{};
    int totalScore_{};   // 스테이지가 바뀌어도 초기화하지 않는다.
    std::chrono::steady_clock::time_point startedAt_{};
};

#endif
