// score.cpp
// 점수, 길이, 아이템 획득 수, Gate 사용 수, 경과 시간을 기록하고 점수판을 출력한다.

#include "score.h"

#include <algorithm>

void Score::reset()
{
    // 스테이지별 카운터만 초기화한다. totalScore_는 게임 전체에 걸쳐 유지된다.
    currentLength_ = 0;
    maxLength_ = 0;
    growthCount_ = 0;
    poisonCount_ = 0;
    gateCount_ = 0;
    startTimer();
}

void Score::updateLength(int currentLength)
{
    // 최대 길이는 B 미션 값이므로 한 번 증가하면 감소하지 않는다.
    currentLength_ = currentLength;
    maxLength_ = std::max(maxLength_, currentLength);
}

void Score::addGrowth()
{
    ++growthCount_;
}

void Score::addPoison()
{
    ++poisonCount_;
}

void Score::addGate()
{
    ++gateCount_;
}

void Score::addPoints(int n)
{
    totalScore_ += n;
}

void Score::startTimer()
{
    startedAt_ = std::chrono::steady_clock::now();
}

int Score::elapsedSeconds() const
{
    using namespace std::chrono;
    // 단조 증가 시계를 사용하면 게임 중 시스템 시간이 바뀌어도 영향을 받지 않는다.
    return static_cast<int>(
        duration_cast<seconds>(steady_clock::now() - startedAt_).count());
}

int Score::currentLength() const
{
    return currentLength_;
}

int Score::maxLength() const
{
    return maxLength_;
}

int Score::growthCount() const
{
    return growthCount_;
}

int Score::poisonCount() const
{
    return poisonCount_;
}

int Score::gateCount() const
{
    return gateCount_;
}

int Score::totalScore() const
{
    return totalScore_;
}

void Score::draw(WINDOW* window, int startY, int startX, int highScore) const
{
    // 맵과 겹치지 않도록 출력 위치는 호출자가 정한다.
    mvwprintw(window, startY,     startX, "Score Board");
    mvwprintw(window, startY + 1, startX, "Score:  %d   ", totalScore_);
    mvwprintw(window, startY + 2, startX, "Best:   %d   ", highScore);
    mvwprintw(window, startY + 3, startX, "---");
    mvwprintw(window, startY + 4, startX, "B: %d / %d  ",
              currentLength_, maxLength_);
    mvwprintw(window, startY + 5, startX, "+: %d", growthCount_);
    mvwprintw(window, startY + 6, startX, "-: %d", poisonCount_);
    mvwprintw(window, startY + 7, startX, "G: %d", gateCount_);
    mvwprintw(window, startY + 8, startX, "Time: %d  ", elapsedSeconds());
}
