#include <ncurses.h>

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <random>
#include <utility>
#include <vector>

#include "item.h"
#include "map.h"
#include "snake.h"

class Game {
public:
    static constexpr int TickMilliseconds = 120;
    static constexpr int RegenerationSeconds = 10;

    Game()
        : snake_(map_)
    {
        regenerateItemsAndGates();
    }

    void run()
    {
        WINDOW* window = newwin(GameMap::Size + 4, GameMap::DrawWidth + 2, 0, 0);
        map_.draw(window, snake_.body(), itemManager_.items(), snake_.length());

        bool running = true;

        while (running)
        {
            const int input = getch();

            if (input == 'q')
            {
                break;
            }

            handleInput(input);

            if (shouldRegenerate())
            {
                regenerateItemsAndGates();
            }

            running = update();
            map_.draw(window, snake_.body(), itemManager_.items(), snake_.length());
        }

        if (!running)
        {
            showGameOver(window);
        }

        delwin(window);
    }

private:
    void handleInput(int input)
    {
        switch (input)
        {
            case KEY_UP:
            case 'w':
            case 'W':
                snake_.changeDirection(Snake::Direction::Up);
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                snake_.changeDirection(Snake::Direction::Down);
                break;
            case KEY_LEFT:
            case 'a':
            case 'A':
                snake_.changeDirection(Snake::Direction::Left);
                break;
            case KEY_RIGHT:
            case 'd':
            case 'D':
                snake_.changeDirection(Snake::Direction::Right);
                break;
            default:
                break;
        }
    }

    bool update()
    {
        std::pair<int, int> next = snake_.nextHead();

        if (map_.isGate(next.first, next.second))
        {
            next = exitGatePosition(next);
        }
        else if (map_.isWall(next.first, next.second))
        {
            return false;
        }

        if (snake_.hitsSelf(next))
        {
            return false;
        }

        bool consumed = false;
        const Item::Type type = itemManager_.consumeAt(next.first, next.second, consumed);
        const bool grow = consumed && type == Item::Type::Growth;

        snake_.moveTo(next, grow);

        if (consumed && type == Item::Type::Poison)
        {
            snake_.shrink();

            if (snake_.length() < Snake::MinimumLength)
            {
                return false;
            }
        }

        return true;
    }

    bool shouldRegenerate() const
    {
        using namespace std::chrono;
        const auto now = steady_clock::now();
        return duration_cast<seconds>(now - lastRegeneration_).count() >= RegenerationSeconds;
    }

    void regenerateItemsAndGates()
    {
        map_.clearGates();
        createGates();
        itemManager_.regenerate(map_, snake_.body());
        lastRegeneration_ = std::chrono::steady_clock::now();
    }

    void createGates()
    {
        std::vector<std::pair<int, int>> wallPositions = map_.getWallPositionsForGate();

        if (wallPositions.size() < 2)
        {
            return;
        }

        static std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
        std::shuffle(wallPositions.begin(), wallPositions.end(), generator);
        gates_.clear();
        gates_.push_back(wallPositions[0]);
        gates_.push_back(wallPositions[1]);

        map_.setGate(gates_[0]);
        map_.setGate(gates_[1]);
    }

    std::pair<int, int> exitGatePosition(const std::pair<int, int>& entrance)
    {
        const std::pair<int, int> exit = entrance == gates_[0] ? gates_[1] : gates_[0];

        if (exit.first == 0)
        {
            snake_.setDirection(Snake::Direction::Down);
            return {exit.first + 1, exit.second};
        }
        if (exit.first == GameMap::Size - 1)
        {
            snake_.setDirection(Snake::Direction::Up);
            return {exit.first - 1, exit.second};
        }
        if (exit.second == 0)
        {
            snake_.setDirection(Snake::Direction::Right);
            return {exit.first, exit.second + 1};
        }

        snake_.setDirection(Snake::Direction::Left);
        return {exit.first, exit.second - 1};
    }

    void showGameOver(WINDOW* window) const
    {
        wclear(window);
        box(window, 0, 0);
        mvwprintw(window, GameMap::Size / 2 - 1, GameMap::DrawWidth / 2 - 5, "GAME OVER");
        mvwprintw(window, GameMap::Size / 2 + 1, GameMap::DrawWidth / 2 - 10,
                  "Final Length : %d", snake_.length());
        mvwprintw(window, GameMap::Size / 2 + 3, GameMap::DrawWidth / 2 - 14,
                  "Press any key to exit");
        wrefresh(window);
        nodelay(stdscr, FALSE);
        getch();
    }

    GameMap map_;
    Snake snake_;
    ItemManager itemManager_;
    std::vector<std::pair<int, int>> gates_;
    std::chrono::steady_clock::time_point lastRegeneration_{};
};

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(Game::TickMilliseconds);
    setColor();
    refresh();

    Game game;
    game.run();

    endwin();
    return 0;
}
