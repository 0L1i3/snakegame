// mission.cpp
// 각 스테이지의 목표 조건을 Score와 비교하고 Mission Board를 출력한다.

#include "mission.h"

#include "score.h"

Mission::Mission() = default;

Mission::Mission(int targetLength,
                 int targetGrowth,
                 int targetPoison,
                 int targetGate)
    // 이 미션 객체의 목표 값을 저장한다.
    : targetLength_(targetLength),
      targetGrowth_(targetGrowth),
      targetPoison_(targetPoison),
      targetGate_(targetGate)
{
}

bool Mission::isLengthAchieved(const Score& score) const
{
    return score.maxLength() >= targetLength_;
}

bool Mission::isGrowthAchieved(const Score& score) const
{
    return score.growthCount() >= targetGrowth_;
}

bool Mission::isPoisonAchieved(const Score& score) const
{
    return score.poisonCount() >= targetPoison_;
}

bool Mission::isGateAchieved(const Score& score) const
{
    return score.gateCount() >= targetGate_;
}

bool Mission::isCompleted(const Score& score) const
{
    // 모든 미션 조건을 만족해야 다음 스테이지로 넘어간다.
    return isLengthAchieved(score) &&
           isGrowthAchieved(score) &&
           isPoisonAchieved(score) &&
           isGateAchieved(score);
}

void Mission::draw(WINDOW* window,
                   int startY,
                   int startX,
                   const Score& score) const
{
    // 과제 예시와 맞게 (v) 표시로 달성 여부를 보여준다.
    mvwprintw(window, startY, startX, "Mission");
    mvwprintw(window, startY + 2, startX, "B: %d (%c)",
              targetLength_, isLengthAchieved(score) ? 'v' : ' ');
    mvwprintw(window, startY + 3, startX, "+: %d (%c)",
              targetGrowth_, isGrowthAchieved(score) ? 'v' : ' ');
    mvwprintw(window, startY + 4, startX, "-: %d (%c)",
              targetPoison_, isPoisonAchieved(score) ? 'v' : ' ');
    mvwprintw(window, startY + 5, startX, "G: %d (%c)",
              targetGate_, isGateAchieved(score) ? 'v' : ' ');
}
