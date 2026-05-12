#include <ncurses.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#define MAP_SIZE 21
using namespace std;

vector<pair<int, int>> place_snake();
void set_color();
void move_snake(vector<pair<int, int>>& snake_pos, int userInput, int length);
void fill_board(WINDOW* win, vector<pair<int, int>>& snake_pos, int length);


int original_map_data[MAP_SIZE][MAP_SIZE] = {
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}
};

int main()
{
    

    int length = 3;
    int userInput;
    vector<pair<int, int>> snake_pos;
    WINDOW *gameboard;

    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    set_color();
    
    refresh();

    gameboard = newwin(MAP_SIZE, MAP_SIZE, 0, 0);
    snake_pos = place_snake();
    
    do
    {
        userInput = getch();
        move_snake(snake_pos, userInput, length);

        fill_board(gameboard, snake_pos, length);
    } while(userInput != 'q');

    getch();
    delwin(gameboard);
    endwin();

    return 0;
}

vector<pair<int, int>> place_snake()
{
    // int startpos_x, startpos_y;
    // vector<pair<int, int>> snake_pos;

    // srand(time(NULL));
    // startpos_x = rand() % (BOARD_SIZE - 2) + 1;
    // startpos_y = rand() % (BOARD_SIZE - 2) + 1;

    // snake_pos.push_back({startpos_y, startpos_x});

    // wattron(gameboard, COLOR_PAIR(1));
    // mvwprintw(gameboard, startpos_y, startpos_x, "3");
    // wattron(gameboard, COLOR_PAIR(2));
    // for (int i = 1; i < 3; i++)
    // {
    //     snake_pos.push_back({startpos_y, startpos_x + i});
    //     mvwprintw(gameboard, startpos_y, startpos_x + i, "4");
    // }
    // wattroff(gameboard, COLOR_PAIR(2));

    // wborder(gameboard, '1', '1', '1', '1', '2','2','2','2');
    // wbkgd(gameboard, '0');

    // wrefresh(gameboard);

    int map_data[MAP_SIZE][MAP_SIZE];

    // 원본 맵 복사
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            map_data[i][j] = original_map_data[i][j];
        }
    }

    vector<pair<int, int>> snake_pos;
    int hx, hy, dir;
    srand((unsigned)time(nullptr));
    int dx[4] = {0, 0, -1, 1};
    int dy[4] = {-1, 1, 0, 0};
    
    hy = 1 + rand() % (MAP_SIZE - 2);
    hx = 1 + rand() % (MAP_SIZE - 2);
    map_data[hy][hx] = 3;
    snake_pos.push_back({hy, hx});

    while (true) {     
        dir = rand() % 4; 
        int t1y = hy + dy[dir], t1x = hx + dx[dir]; 
        int t2y = hy + 2 * dy[dir], t2x = hx + 2 * dx[dir];
        if (t1y < 1 || t1y > MAP_SIZE - 2 || t1x < 1 || t1x > MAP_SIZE - 2) continue; 
        if (t2y < 1 || t2y > MAP_SIZE - 2 || t2x < 1 || t2x > MAP_SIZE - 2) continue; 
        map_data[t1y][t1x] = 4; map_data[t2y][t2x] = 4; 
        snake_pos.push_back({t1y, t1x});
        snake_pos.push_back({t2y, t2x});
        break;
    }

    return snake_pos;
}

void set_color()
{
    start_color();
    init_pair(1, COLOR_RED, COLOR_RED);
    init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
}

void move_snake(vector<pair<int, int>>& snake_pos, int userInput, int length)
{
    // bool isleft = (snake_pos[0].second < snake_pos[1].second ? true : false);
    // bool isup = (snake_pos[0].first < snake_pos[1].first ? true : false);
    
    for (int i = length - 1; i > 0; i--)
    {
        snake_pos[i] = snake_pos[i - 1];
    }

    switch(userInput)
    {
        case 'w':
        case KEY_UP:
            snake_pos[0].first--;

            break;
        case 'a':
        case KEY_LEFT:
            snake_pos[0].second--;

            break;
        case 's':
        case KEY_DOWN:
            snake_pos[0].first++;    

            break;
        case 'd':
        case KEY_RIGHT:
            snake_pos[0].second++;

            break;
    }
}

void fill_board(WINDOW* gameboard,
                vector<pair<int, int>>& snake_pos,
                int length)
{
    int map_data[MAP_SIZE][MAP_SIZE];

    // 원본 맵 복사
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            map_data[i][j] = original_map_data[i][j];
        }
    }

    // snake 위치 반영
    map_data[snake_pos[0].first][snake_pos[0].second] = 3;

    for (int i = 1; i < length; i++)
    {
        map_data[snake_pos[i].first][snake_pos[i].second] = 4;
    }

    // 화면 지우기
    wclear(gameboard);

    // 전체 맵 출력
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            switch(map_data[i][j])
            {
                case 0:
                    mvwprintw(gameboard, i, j, " ");
                    break;

                case 1:
                    mvwprintw(gameboard, i, j, "#");
                    break;

                case 2:
                    mvwprintw(gameboard, i, j, "+");
                    break;

                case 3:
                    wattron(gameboard, COLOR_PAIR(1));
                    mvwprintw(gameboard, i, j, " ");
                    wattroff(gameboard, COLOR_PAIR(1));
                    break;

                case 4:
                    wattron(gameboard, COLOR_PAIR(2));
                    mvwprintw(gameboard, i, j, " ");
                    wattroff(gameboard, COLOR_PAIR(2));
                    break;
            }
        }
    }

    wrefresh(gameboard);
}

// void fill_board(WINDOW* gameboard, vector<pair<int, int>>& snake_pos, int length)
// {
//     wclear(gameboard);
//     wattron(gameboard, COLOR_PAIR(1));
//     mvwprintw(gameboard, snake_pos[0].first, snake_pos[0].second, "3");
//     wattron(gameboard, COLOR_PAIR(2));
//     for (int i = 1; i < length; i++)
//     {
//         mvwprintw(gameboard, snake_pos[i].first, snake_pos[i].second, "4");
//     }
//     wattroff(gameboard, COLOR_PAIR(2));

//     wborder(gameboard, '1', '1', '1', '1', '2','2','2','2');
//     // wbkgd(gameboard, '0');

//     wrefresh(gameboard);
// }