#include "game.h"

Game::Game()
    : snake_(map_)
    , pacman_(map_)
{
    // 실행 함수가 바로 그릴 수 있도록 첫 스테이지를 미리 구성한다.
    startStage();
}

void Game::run()
{
    // 맵은 한 칸을 두 글자로 출력하므로, 오른쪽 사이드바까지 포함한 너비로 창을 만든다.
    WINDOW* window = newwin(GameMap::Size + 4,
                            GameMap::DrawWidth + SidebarWidth,
                            0,
                            0);
    draw(window);

    bool running = true;

    while (running)
    {
        // 키 입력 함수는 제한 시간 설정의 영향을 받으므로 입력이 없어도 루프가 계속 진행된다.
        const int input = getch();

        if (input == 'q' || input == 'Q')
        {
            break;
        }

        if (!handleInput(input, window))
        {
            // 반대 방향 입력은 치트 모드가 아닐 때 게임 오버 규칙이다.
            running = false;
            break;
        }

        if (shouldRegenerate())
        {
            regenerateObjects();
        }

        running = update();
        draw(window);
    }

    if (gameClear_)
    {
        // 마지막 미션까지 완료한 경우다.
        showGameClear(window);
    }
    else if (!running)
    {
        showGameOver(window);
    }

    delwin(window);
}

bool Game::handleInput(int input, WINDOW* window)
{
    switch (input)
    {
        case KEY_UP:
        case 'w':
        case 'W':
            // 방향 변경 함수는 금지된 반대 방향 입력이면 거짓을 반환한다.
            return snake_.changeDirection(Snake::Direction::Up) || invincible_;
        case KEY_DOWN:
        case 's':
        case 'S':
            return snake_.changeDirection(Snake::Direction::Down) || invincible_;
        case KEY_LEFT:
        case 'a':
        case 'A':
            return snake_.changeDirection(Snake::Direction::Left) || invincible_;
        case KEY_RIGHT:
        case 'd':
        case 'D':
            return snake_.changeDirection(Snake::Direction::Right) || invincible_;
        case 't':
        case 'T':
            // 치트키는 숨겨진 입력이 아니라 화면에 직접 입력받는다.
            promptCheatCode(window);
            return true;
        default:
            return true;
    }
}

void Game::promptCheatCode(WINDOW* window)
{
    char input[32] = {};

    // 치트키 입력 중에는 틱 기반 입력을 잠시 멈추고 문자열 입력을 받는다.
    timeout(-1);
    echo();
    curs_set(1);

    mvwprintw(window, GameMap::Size + 3, 0, "Cheat Code: ");
    wclrtoeol(window);
    wrefresh(window);
    wgetnstr(window, input, static_cast<int>(sizeof(input)) - 1);

    noecho();
    curs_set(0);
    timeout(TickMilliseconds);

    // 치트 모드는 한 번 켜지면 스테이지가 바뀌어도 유지된다.
    if (std::string(input) == "KMUCPP2026")
    {
        invincible_ = true;
        statusMessage_ = "Cheat enabled: Invincible";
    }
    else
    {
        statusMessage_ = "Wrong cheat code";
    }
}

bool Game::update()
{
    // 이동 전에 만료된 아이템을 제거하고 빈 아이템 슬롯을 채운다.
    itemManager_.update(map_, snake_.body());
    pacman_.update(map_, snake_.body(), itemManager_, portalManager_);

    std::pair<int, int> next = snake_.nextHead();
    bool usedPortal = false;

    if (portalManager_.isPortal(next.first, next.second))
    {
        // 포탈에 들어가면 반대쪽 포탈의 출구 칸으로 머리가 이동한다.
        Snake::Direction newDirection = snake_.direction();
        portalManager_.setSnakeUsingPortal(true);
        next = portalManager_.calculateExitPosition(
            next, snake_.direction(), newDirection, map_, snake_);

        if (!map_.isInside(next.first, next.second))
        {
            // 유효한 포탈 출구가 없으면 치트 모드에서는 이동하지 않고 살아남는다.
            portalManager_.setSnakeUsingPortal(false);
            return invincible_;
        }

        snake_.setDirection(newDirection);
        score_.addGate();
        usedPortal = true;
    }
    else if (!map_.isInside(next.first, next.second) ||
             map_.isWall(next.first, next.second))
    {
        if (invincible_)
        {
            // 무적 모드에서는 벽에 부딪혀도 죽지 않고 제자리에 머문다.
            score_.updateLength(snake_.length());
            return advanceStageIfComplete();
        }

        return false;
    }

    if (!map_.isInside(next.first, next.second) ||
        map_.isWall(next.first, next.second) ||
        snake_.hitsSelf(next) ||
        next == pacman_.position())
    {
        if (invincible_)
        {
            // 무적 모드에서는 자기 몸 충돌도 제자리에 머무는 방식으로 무시한다.
            score_.updateLength(snake_.length());
            return advanceStageIfComplete();
        }

        return false;
    }

    bool consumed = false;
    const Item::Type type = itemManager_.consumeAt(next.first, next.second, consumed);
    const bool grow = consumed && type == Item::Type::Growth;

    // 일반 이동은 새 머리를 추가하고, 성장 아이템을 먹으면 꼬리를 유지한다.
    snake_.moveTo(next, grow);

    if (consumed && type == Item::Type::Growth)
    {
        score_.addGrowth();
    }
    else if (consumed && type == Item::Type::Poison)
    {
        score_.addPoison();
        // 무적 모드에서는 독 아이템으로 최소 길이보다 짧아지지 않게 한다.
        if (snake_.length() > Snake::MinimumLength || !invincible_)
        {
            snake_.shrink();
        }

        if (snake_.length() < Snake::MinimumLength && !invincible_)
        {
            portalManager_.setSnakeUsingPortal(false);
            return false;
        }
    }

    if (usedPortal)
    {
        // 머리가 포탈을 빠져나온 뒤에는 포탈 재생성이 다시 가능하다.
        portalManager_.setSnakeUsingPortal(false);
    }

    score_.updateLength(snake_.length());
    return advanceStageIfComplete();
}

bool Game::shouldRegenerate() const
{
    using namespace std::chrono;
    const auto now = steady_clock::now();
    // 하나의 재생성 타이머로 포탈과 아이템을 함께 관리한다.
    return duration_cast<seconds>(now - lastRegeneration_).count() >= RegenerationSeconds;
}

bool Game::advanceStageIfComplete()
{
    if (!mission_.isCompleted(score_))
    {
        return true;
    }

    if (stageManager_.isLastStage())
    {
        // 마지막 스테이지 미션을 완료하면 게임 클리어 처리한다.
        gameClear_ = true;
        return false;
    }

    // 스테이지마다 맵, 미션, 점수 카운터, 아이템, 포탈을 새로 설정한다.
    stageManager_.nextStage();
    startStage();
    return true;
}

void Game::startStage()
{
    stageManager_.loadCurrentStage(map_);
    snake_ = Snake(map_);
    pacman_.reset(map_, snake_.body());
    score_.reset();
    score_.updateLength(snake_.length());
    mission_ = stageManager_.currentMission();
    // 스테이지 초기화 시 포탈 사용 중 상태가 남아 있지 않도록 한다.
    portalManager_.setSnakeUsingPortal(false);
    regenerateObjects();
}

void Game::regenerateObjects()
{
    // 포탈 관리자는 뱀이 포탈을 통과 중이면 재생성을 거부한다.
    portalManager_.generate(map_);
    itemManager_.regenerate(map_, snake_.body());
    lastRegeneration_ = std::chrono::steady_clock::now();
}

void Game::draw(WINDOW* window) const
{
    // 맵 객체가 보드를 그린 뒤 게임 객체가 오른쪽 화면 요소를 추가로 그린다.
    map_.draw(window,
              snake_.body(),
              itemManager_.items(),
              snake_.length(),
              pacman_.position());
    mvwprintw(window, 1, GameMap::DrawWidth + 4, "Stage: %d / %d",
              stageManager_.currentStage(), StageManager::TotalStages);
    mvwprintw(window, 2, GameMap::DrawWidth + 4, "Cheat: %s",
              invincible_ ? "ON" : "OFF");
    score_.draw(window, 4, GameMap::DrawWidth + 4);
    mission_.draw(window, 14, GameMap::DrawWidth + 4, score_);
    mvwprintw(window, GameMap::Size + 2, 0,
              "Move: Arrow keys / WASD    Cheat: t    Quit: q");
    if (!statusMessage_.empty())
    {
        mvwprintw(window, GameMap::Size + 3, 0, "%s", statusMessage_.c_str());
    }
    wrefresh(window);
}

void Game::showGameOver(WINDOW* window) const
{
    // 게임 화면을 지우고 키 입력을 기다리는 결과 화면을 출력한다.
    wclear(window);
    box(window, 0, 0);
    mvwprintw(window, GameMap::Size / 2 - 2, GameMap::DrawWidth / 2 - 5, "GAME OVER");
    mvwprintw(window, GameMap::Size / 2, GameMap::DrawWidth / 2 - 10,
              "Final Length : %d", snake_.length());
    mvwprintw(window, GameMap::Size / 2 + 1, GameMap::DrawWidth / 2 - 10,
              "Reached Stage: %d", stageManager_.currentStage());
    mvwprintw(window, GameMap::Size / 2 + 3, GameMap::DrawWidth / 2 - 14,
              "Press any key to exit");
    wrefresh(window);
    nodelay(stdscr, FALSE);
    getch();
}

void Game::showGameClear(WINDOW* window) const
{
    // 마지막 스테이지 미션을 완료했을 때만 출력된다.
    wclear(window);
    box(window, 0, 0);
    mvwprintw(window, GameMap::Size / 2 - 2, GameMap::DrawWidth / 2 - 5, "GAME CLEAR");
    mvwprintw(window, GameMap::Size / 2, GameMap::DrawWidth / 2 - 15,
              "All missions completed!");
    mvwprintw(window, GameMap::Size / 2 + 2, GameMap::DrawWidth / 2 - 14,
              "Press any key to exit");
    wrefresh(window);
    nodelay(stdscr, FALSE);
    getch();
}
