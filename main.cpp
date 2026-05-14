#include <ncurses.h>
#include <vector>

#include "snake.h"
#include "map.h"
#include "item.h"

using namespace std;

int main()
{
    int userInput;

    vector<pair<int, int>> snake_pos;
    vector<Item> items;

    WINDOW* gameboard;

    int dirY = 0;
    int dirX = -1;

    initscr();

    keypad(stdscr, TRUE);

    curs_set(0);

    noecho();

    timeout(200);

    set_color();

    refresh();

    gameboard = newwin(MAP_SIZE, MAP_SIZE*2, 0, 0); //찌부방지

    snake_pos = place_snake(dirY, dirX);

    create_items(items);

    while (true)
    {
        userInput = getch();

        if (userInput == 'q')
            break;

        if (userInput != ERR)
        {
            process_input(userInput, dirY, dirX);
        }

        move_snake(snake_pos, items, dirY, dirX);

        fill_board(gameboard, snake_pos, items);
    }

    delwin(gameboard);

    endwin();

    return 0;
}
