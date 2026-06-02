// mission.h
// 스테이지 목표값과 목표 달성 판정/출력 인터페이스를 선언한다.

#ifndef MISSION_H
#define MISSION_H

#include <ncurses.h>

class Score;

// 한 스테이지의 목표 값을 저장하고 Score와 비교해 달성 여부를 판단한다.
class Mission {
public:
    Mission();
    // 목표 값은 최대 길이, Growth 획득 수, Poison 획득 수, Gate 사용 수 순서다.
    Mission(int targetLength, int targetGrowth, int targetPoison, int targetGate);

    // 각 항목별 달성 여부는 Mission Board의 표시에도 사용된다.
    bool isLengthAchieved(const Score& score) const;
    bool isGrowthAchieved(const Score& score) const;
    bool isPoisonAchieved(const Score& score) const;
    bool isGateAchieved(const Score& score) const;
    // 네 가지 목표를 모두 달성해야 스테이지를 클리어한다.
    bool isCompleted(const Score& score) const;

    // 목표 값과 달성 표시를 화면에 출력한다.
    void draw(WINDOW* window, int startY, int startX, const Score& score) const;

private:
    int targetLength_{};
    int targetGrowth_{};
    int targetPoison_{};
    int targetGate_{};
};

#endif
