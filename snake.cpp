#include <ncurses.h>
#include <deque>
#include <cstdlib>
#include <ctime>

const int MAP_SIZE = 21;
const int CELL_W   = 2;

struct Pos { int y, x; };

void init_color() {
    start_color();
    init_pair(1, COLOR_WHITE,  COLOR_WHITE);  // 일반 벽
    init_pair(2, COLOR_CYAN,   COLOR_CYAN);   // 모서리 벽
    init_pair(3, COLOR_YELLOW, COLOR_RED);    // 뱀 머리
    init_pair(4, COLOR_GREEN,  COLOR_GREEN);  // 뱀 몸통
    init_pair(5, COLOR_BLACK,  COLOR_BLACK);  // 빈 공간
}

class Snake {
public:
    std::deque<Pos> body;
    int dirY, dirX;
    bool alive;

    Snake() : alive(true) {
        srand((unsigned)time(nullptr));
        int dx[4] = { 0,  0, -1, 1};
        int dy[4] = {-1,  1,  0, 0};

        int hx, hy, dir;
        while (true) {
            hy  = 1 + rand() % (MAP_SIZE - 2);
            hx  = 1 + rand() % (MAP_SIZE - 2);
            dir = rand() % 4;

            int t1y = hy + dy[dir], t1x = hx + dx[dir];
            int t2y = hy + 2*dy[dir], t2x = hx + 2*dx[dir];

            if (t1y < 1 || t1y > MAP_SIZE-2 || t1x < 1 || t1x > MAP_SIZE-2) continue;
            if (t2y < 1 || t2y > MAP_SIZE-2 || t2x < 1 || t2x > MAP_SIZE-2) continue;

            body.push_back({hy,  hx });
            body.push_back({t1y, t1x});
            body.push_back({t2y, t2x});
            dirY = -dy[dir];
            dirX = -dx[dir];
            break;
        }
    }

    Pos nextHead() const {
        return {body.front().y + dirY, body.front().x + dirX};
    }

    void move() {
        body.push_front(nextHead());
        body.pop_back();
    }

    void changeDirection(int key) {
        switch (key) {
            case KEY_UP:    if (dirY !=  1) { dirY = -1; dirX =  0; } break;
            case KEY_DOWN:  if (dirY != -1) { dirY =  1; dirX =  0; } break;
            case KEY_LEFT:  if (dirX !=  1) { dirY =  0; dirX = -1; } break;
            case KEY_RIGHT: if (dirX != -1) { dirY =  0; dirX =  1; } break;
        }
    }
};

class SnakeGame {
private:
    int     mapData[MAP_SIZE][MAP_SIZE];
    Snake   snake;
    WINDOW *win;

public:
    explicit SnakeGame(WINDOW *w) : win(w) {
        for (int i = 0; i < MAP_SIZE; i++)
            for (int j = 0; j < MAP_SIZE; j++) {
                bool corner = (i == 0 || i == MAP_SIZE-1) && (j == 0 || j == MAP_SIZE-1);
                bool edge   = (i == 0 || i == MAP_SIZE-1 || j == 0 || j == MAP_SIZE-1);
                mapData[i][j] = corner ? 2 : (edge ? 1 : 0);
            }
    }

    void update() {
        if (!snake.alive) return;

        Pos next = snake.nextHead();
        if (mapData[next.y][next.x] != 0) {
            // 벽 충돌 → 뱀 제거
            snake.body.clear();
            snake.alive = false;
            return;
        }
        snake.move();
    }

    void draw() {
        for (int i = 0; i < MAP_SIZE; i++) {
            for (int j = 0; j < MAP_SIZE; j++) {
                int cp = 5;
                if      (mapData[i][j] == 1) cp = 1;
                else if (mapData[i][j] == 2) cp = 2;

                for (int k = 0; k < (int)snake.body.size(); k++) {
                    if (snake.body[k].y == i && snake.body[k].x == j) {
                        cp = (k == 0) ? 3 : 4;
                        break;
                    }
                }

                wattron(win, COLOR_PAIR(cp));
                mvwaddstr(win, i, j * CELL_W, "  ");
                wattroff(win, COLOR_PAIR(cp));
            }
        }
        wrefresh(win);
    }

    void processInput(int key) {
        if (snake.alive)
            snake.changeDirection(key);
    }
};

int main() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(200);
    init_color();

    WINDOW *display = newwin(MAP_SIZE, MAP_SIZE * CELL_W, 0, 0);
    keypad(display, TRUE);

    SnakeGame game(display);

    while (true) {
        int ch = getch();
        if (ch == 'q') break;

        game.processInput(ch);
        game.update();
        game.draw();
    }

    delwin(display);
    endwin();
    return 0;
}