// main.cpp
// ncurses 환경을 초기화하고 Game 객체를 실행하는 프로그램 진입점이다.

#include <cstdlib>
#include <ctime>

#include <ncurses.h>

#include "game.h"
#include "map.h"

// 프로그램 시작 지점이다. ncurses 초기화와 종료를 담당하고 실제 게임은 Game에 맡긴다.
int main()
{
    // 뱀, 아이템, 포탈 위치를 랜덤으로 정하기 위해 난수 시드를 설정한다.
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // 엔커시스를 실시간 입력 모드로 초기화한다.
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(Game::TickMilliseconds);
    setColor();
    refresh();

    // 터미널 설정이 끝난 뒤 실제 게임 루프를 실행한다.
    Game game;
    game.run();

    // 프로그램 종료 전에 터미널 상태를 원래대로 복구한다.
    endwin();
    return 0;
}
