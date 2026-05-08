#include <ncurses.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#define MAP_SIZE 21
using namespace std;

vector<pair<int, int>> place_snake();
void set_color();
void move_snake(vector<pair<int, int>>& snake_pos, int& dirY, int& dirX);
void process_input(int userInput, int& dirY, int& dirX);
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

    // dirY, dirX(방향 변수)를 선언
    int dirY = 0, dirX = -1;

    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    set_color();
    timeout(200);   // Tick 주기 200ms로 설정

    refresh();

    gameboard = newwin(MAP_SIZE, MAP_SIZE, 0, 0);
    snake_pos = place_snake();

    while (true)
    {
        userInput = getch();

        if (userInput == 'q') break;

        //키 입력이 있으면 방향 갱신, 없으면 현 방향 유지
        if (userInput != ERR)
            process_input(userInput, dirY, dirX);

        // ★ TICK마다 현재 방향으로 뱀 이동,, 200ms마다 뱀 현재 방향으로 이동
        move_snake(snake_pos, dirY, dirX);

        fill_board(gameboard, snake_pos, length);
    }

    delwin(gameboard);
    endwin();

    return 0;
}

vector<pair<int, int>> place_snake()
{
    int map_data[MAP_SIZE][MAP_SIZE];

    for (int i = 0; i < MAP_SIZE; i++)
        for (int j = 0; j < MAP_SIZE; j++)
            map_data[i][j] = original_map_data[i][j];

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

//방향 입력 처리, 반대 방향은 무시
void process_input(int userInput, int& dirY, int& dirX)
{
    switch (userInput)
    {
        case 'w':
        case KEY_UP:
            if (dirY != 1)  { dirY = -1; dirX = 0; }   //아래 방향일 때 무시
            break;
        case 's':
        case KEY_DOWN:
            if (dirY != -1) { dirY = 1;  dirX = 0; }   //위 방향일 때 무시
            break;
        case 'a':
        case KEY_LEFT:
            if (dirX != 1)  { dirY = 0;  dirX = -1; }  //오른쪽 방향일 때 무시
            break;
        case 'd':
        case KEY_RIGHT:
            if (dirX != -1) { dirY = 0;  dirX = 1; }   //왼쪽 방향일 때 무시
            break;
    }
}

//방향 벡터로 뱀 이동
void move_snake(vector<pair<int, int>>& snake_pos, int& dirY, int& dirX)
{
    int length = (int)snake_pos.size();

    //꼬리부터 앞 노드 위치로 당기기
    for (int i = length - 1; i > 0; i--)
        snake_pos[i] = snake_pos[i - 1];

    //머리를 현재 방향으로 한 칸 전진
    snake_pos[0].first  += dirY;
    snake_pos[0].second += dirX;
}

void fill_board(WINDOW* gameboard,
                vector<pair<int, int>>& snake_pos,
                int length)
{
    int map_data[MAP_SIZE][MAP_SIZE];

    for (int i = 0; i < MAP_SIZE; i++)
        for (int j = 0; j < MAP_SIZE; j++)
            map_data[i][j] = original_map_data[i][j];

    map_data[snake_pos[0].first][snake_pos[0].second] = 3;
    for (int i = 1; i < length; i++)
        map_data[snake_pos[i].first][snake_pos[i].second] = 4;

    wclear(gameboard);

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
