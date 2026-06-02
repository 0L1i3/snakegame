// game.h
// 게임 진행을 총괄하는 Game 클래스와 주요 시간/화면 설정을 선언한다.

#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <string>
#include <fstream>

#include <ncurses.h>

#include "item.h"
#include "map.h"
#include "mission.h"
#include "pacman.h"
#include "portal.h"
#include "score.h"
#include "snake.h"
#include "stage.h"

// 전체 게임 루프를 관리하고 Map, Snake, Item, Portal, Score, Mission, Stage를 연결한다.
// 세부 규칙은 각 전담 객체에 맡긴다.
class Game {
public:
    // 한 틱의 시간이며 입력 대기 시간과 뱀 이동 속도에 사용된다.
    static constexpr int TickMilliseconds = 180;
    // 아이템과 포탈을 재생성하는 시간 간격이다.
    static constexpr int RegenerationSeconds = 10;

    Game();

    // 엔커시스 기반 게임 루프를 시작한다.
    void run();

private:
    // 맵 오른쪽에 점수판과 상태 메시지를 출력하기 위한 여분 너비다.
    static constexpr int SidebarWidth = 36;

    // 입력을 처리한다. 반대 방향 입력처럼 입력 자체가 게임 오버이면 false를 반환한다.
    bool handleInput(int input, WINDOW* window);
    // 치트키 입력을 위해 ncurses를 잠시 문자열 입력 모드로 바꾼다.
    void promptCheatCode(WINDOW* window);
    // 한 틱 동안 이동, 충돌, 아이템, 포탈, 미션 판정을 처리한다.
    bool update();
    // 시간 기반 오브젝트를 재생성해야 하는지 확인한다.
    bool shouldRegenerate() const;
    // 미션 완료 시 다음 스테이지로 이동하거나 전체 클리어를 표시한다.
    bool advanceStageIfComplete();
    // 최고 점수를 파일에서 읽고 쓴다.
    void loadHighScore();
    void saveHighScore();

    // 현재 스테이지를 로드하고 스테이지별 게임 상태를 초기화한다.
    void startStage();
    // 포탈과 아이템을 함께 재생성한다.
    void regenerateObjects();
    // 스테이지 시작 시 무적 타이머까지 포함해 전체 초기화한다.
    void initStageObjects();
    // 맵과 오른쪽 UI를 출력한다.
    void draw(WINDOW* window) const;
    // 게임 종료 화면을 출력하고 키 입력을 기다린다.
    void showGameOver(WINDOW* window) const;
    void showGameClear(WINDOW* window) const;

    // 게임을 구성하는 핵심 객체들이다.
    GameMap map_;
    Snake snake_;
    ItemManager itemManager_;
    PortalManager portalManager_;
    Pacman pacman_;
    Score score_;
    Mission mission_;
    StageManager stageManager_;

    // 게임 전체에 적용되는 상태 플래그와 안내 메시지다.
    bool gameClear_{false};
    bool invincible_{false};
    int highScore_{0};
    int lastTimeScoreSecond_{0};
    std::chrono::steady_clock::time_point gameStartedAt_{};
    // 무적 아이템 소비 시 5초간 벽 충돌을 무시한다.
    bool itemInvincibleActive_{false};
    std::chrono::steady_clock::time_point itemInvincibleUntil_{};
    std::string statusMessage_;
    std::chrono::steady_clock::time_point lastRegeneration_{};
};

#endif
