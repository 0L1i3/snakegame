#include "map.h"
#include <ctime>
#include <cstdlib>

using namespace std;

int original_map_data[MAP_SIZE][MAP_SIZE] =
{
    {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2}
};

void set_color()
{
    start_color();

    init_pair(1, COLOR_RED, COLOR_RED);

    init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
}

void fill_board(WINDOW* win,
                vector<pair<int, int>>& snake_pos,
                vector<Item>& items)
{
    int map_data[MAP_SIZE][MAP_SIZE];

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            map_data[i][j] = original_map_data[i][j];
        }
    }

    apply_items(map_data, items);

    map_data[snake_pos[0].first][snake_pos[0].second] = 3;

    for (int i = 1; i < snake_pos.size(); i++)
    {
        map_data[snake_pos[i].first][snake_pos[i].second] = 4;
    }

    wclear(win);

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            switch(map_data[i][j]) //j*2 = 찌부방지
            {
                case 0:
                    mvwprintw(win, i, j*2, "  ");
                    break;

                case 1:
                    mvwprintw(win, i, j*2, "[]");
                    break;

                case 2:
                    mvwprintw(win, i, j*2, "##");
                    break;

                case 3:
                    wattron(win, COLOR_PAIR(1));
                    mvwprintw(win, i, j*2, "  ");
                    wattroff(win, COLOR_PAIR(1));
                    break;

                case 4:
                    wattron(win, COLOR_PAIR(2));
                    mvwprintw(win, i, j*2, "  ");
                    wattroff(win, COLOR_PAIR(2));
                    break;

                case 5:
                    mvwprintw(win, i, j*2, "GG");
                    break;

                case 6:
                    mvwprintw(win, i, j*2, "PP");
                    break;
            }
        }
    }

    mvprintw(22, 0, "Length : %d", (int)snake_pos.size());

    wrefresh(win);
}
