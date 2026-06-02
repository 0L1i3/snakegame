// stage.h
// 스테이지 번호와 스테이지별 Mission/Map 연결 인터페이스를 선언한다.

#ifndef STAGE_H
#define STAGE_H

#include "mission.h"

class GameMap;

// 현재 스테이지 번호를 관리하고 스테이지별 맵/미션 데이터를 연결한다.
class StageManager {
public:
    // 과제 요구사항에 맞춰 최소 4개 스테이지를 제공한다.
    static constexpr int TotalStages = 4;

    StageManager();

    // 스테이지 번호는 화면 표시와 맵 로딩을 위해 1부터 시작한다.
    int currentStage() const;
    bool isLastStage() const;
    void nextStage();

    // 현재 스테이지의 맵과 미션을 적용한다.
    void loadCurrentStage(GameMap& map) const;
    Mission currentMission() const;

private:
    int currentStage_{1};
};

#endif
