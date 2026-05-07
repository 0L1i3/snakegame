#include <ncurses.h>
#include <vector>
#include <deque>

const int MAP_SIZE = 21;

// 방향 구조체
struct Pos {
    int y, x;
};

class Snake {
public:
    std::deque<Pos> body;
    int dirY, dirX; // 현재 이동 방향

    Snake() {
        // 초기 위치: 맵 중앙, 초기 길이 3
        body.push_back({10, 10}); // Head
        body.push_back({10, 11});
        body.push_back({10, 12});
        dirY = 0; dirX = -1; // 초기 방향: 왼쪽
    }

    void move() {
        // 새로운 머리 위치 계산
        Pos nextHead = {body.front().y + dirY, body.front().x + dirX};
        
        // 머리 추가
        body.push_front(nextHead);
        // 꼬리 제거 (이동 효과)    
        body.pop_back();
    }

    void changeDirection(int key) {
        // 반대 방향 이동 방지 로직 포함
        switch(key) {
            case KEY_UP:    if(dirY != 1)  { dirY = -1; dirX = 0; } break;
            case KEY_DOWN:  if(dirY != -1) { dirY = 1;  dirX = 0; } break;
            case KEY_LEFT:  if(dirX != 1)  { dirY = 0;  dirX = -1; } break;
            case KEY_RIGHT: if(dirX != -1) { dirY = 0;  dirX = 1; } break;
        }
    }
};

class SnakeGame {
private:
    int mapData[MAP_SIZE][MAP_SIZE];
    Snake snake;

public:
    SnakeGame() {
        initMap();
    }

    void initMap() {
        for (int i = 0; i < MAP_SIZE; i++) {
            for (int j = 0; j < MAP_SIZE; j++) {
                if ((i == 0 || i == MAP_SIZE - 1) && (j == 0 || j == MAP_SIZE - 1)) mapData[i][j] = 2;
                else if (i == 0 || i == MAP_SIZE - 1 || j == 0 || j == MAP_SIZE - 1) mapData[i][j] = 1;
                else mapData[i][j] = 0;
            }
        }
    }

    void update() {
        snake.move();
        // 벽 충돌 체크 등 로직 추가해야함
    }

    void draw() {
        // 1. 맵 데이터 초기화(스네이크 위치는 매번 새로 그리기 위해 빈 공간으로)
        // 실제로는 맵 데이터와 객체 데이터를 분리하여 관리하는 것이 좋다
        
        for (int i = 0; i < MAP_SIZE; i++) {
            for (int j = 0; j < MAP_SIZE; j++) {
                int val = mapData[i][j];
                // 맵 기본 요소 출력
                if (val == 0) mvprintw(i, j * 2, ". ");
                else if (val == 1) mvprintw(i, j * 2, "X ");
                else if (val == 2) mvprintw(i, j * 2, "+ ");
            }
        }

        // 2. 뱀 출력 (머리와 몸통 구분)
        for (int i = 0; i < snake.body.size(); i++) {
            Pos p = snake.body[i];
            if (i == 0) mvprintw(p.y, p.x * 2, "H "); // Head (3)
            else mvprintw(p.y, p.x * 2, "B ");        // Body (4)
        }
    }

    void processInput(int key) {
        snake.changeDirection(key);
    }
};

int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE); // 화살표 키 입력 허용
    timeout(200);         // Tick 주기 200ms

    SnakeGame game;

    while (true) {
        int ch = getch();
        if (ch == 'q') break;

        game.processInput(ch);
        game.update();
        
        clear();
        game.draw();
        refresh();
    }

    endwin();
    return 0;
}