// stage.cpp
// 현재 스테이지 번호를 관리하고 스테이지별 맵과 미션 데이터를 제공한다.

#include "stage.h"

#include <array>

#include "map.h"

namespace {

const std::array<Mission, StageManager::TotalStages>& stageMissions()
{
    // 1스테이지부터 4스테이지까지의 미션 목표 값이다.
    static const std::array<Mission, StageManager::TotalStages> missions = {{
        Mission(5, 1, 0, 1),
        Mission(7, 2, 1, 1),
        Mission(9, 3, 1, 2),
        Mission(10, 4, 2, 2)
    }};

    return missions;
}

} // 익명 네임스페이스

StageManager::StageManager() = default;

int StageManager::currentStage() const
{
    return currentStage_;
}

bool StageManager::isLastStage() const
{
    // 다음 스테이지로 갈지 게임 클리어로 끝낼지 판단할 때 사용한다.
    return currentStage_ == TotalStages;
}

void StageManager::nextStage()
{
    // 미션 배열 범위를 벗어나지 않도록 마지막 스테이지에서 멈춘다.
    if (!isLastStage())
    {
        ++currentStage_;
    }
}

void StageManager::loadCurrentStage(GameMap& map) const
{
    // 실제 맵 구조 정의는 GameMap이 가지고 있다.
    map.loadStage(currentStage_);
}

Mission StageManager::currentMission() const
{
    // 현재 스테이지 번호는 1부터 시작하지만 배열 인덱스는 0부터 시작한다.
    return stageMissions()[currentStage_ - 1];
}
