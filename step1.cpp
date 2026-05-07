#include <ncurses.h>
#include <cstdlib>
#include <ctime>

#define MAP_SIZE 21

int map_data[MAP_SIZE][MAP_SIZE] = {
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

void place_snake() { 
    int hx, hy, dir;
    srand((unsigned)time(nullptr));
    int dx[4] = {0, 0, -1, 1};
    int dy[4] = {-1, 1, 0, 0}; 
    while (true) { 
        hy = 1 + rand() % (MAP_SIZE - 2);
        hx = 1 + rand() % (MAP_SIZE - 2);
        dir = rand() % 4; 
        int t1y = hy + dy[dir], t1x = hx + dx[dir]; 
        int t2y = hy + 2 * dy[dir], t2x = hx + 2 * dx[dir];
        if (t1y < 1 || t1y > MAP_SIZE - 2 || t1x < 1 || t1x > MAP_SIZE - 2) continue; 
        if (t2y < 1 || t2y > MAP_SIZE - 2 || t2x < 1 || t2x > MAP_SIZE - 2) continue; 
        map_data[hy][hx] = 3; map_data[t1y][t1x] = 4; map_data[t2y][t2x] = 4; 
        break;
    }
}

void init_color() {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_WHITE);
    init_pair(2, COLOR_CYAN, COLOR_CYAN);
    init_pair(3, COLOR_YELLOW, COLOR_RED);    
    init_pair(4, COLOR_GREEN, COLOR_GREEN);
    init_pair(5, COLOR_BLACK, COLOR_BLACK);
}

void draw_map(WINDOW *win) {
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            int val = map_data[y][x];

            int pair = 5;

            if (val >= 1 && val <= 4) pair = val;

            wattron(win, COLOR_PAIR(pair));
            mvwaddch(win, y, x, ' '); 
            wattroff(win, COLOR_PAIR(pair));
        }
    }
    wrefresh(win);
}

int main() 
{
    int row, col;
    
    initscr();
    noecho();
    curs_set(0);
    init_color();
    
    WINDOW *display = newwin(MAP_SIZE, MAP_SIZE, 0, 0);
    
    place_snake();

    while (true)
    {
        draw_map(display);
        
        int ch = getch();
        if (ch == 'q') break;
    }
    
    delwin(display);
    endwin();
    return 0;

}