#include <ncurses.h>

#include "snake.h"

using namespace std;

vector<pair<int, int>> place_snake()
{
    vector<pair<int, int>> snake_pos;

    snake_pos.push_back({10, 10});
    snake_pos.push_back({10, 11});
    snake_pos.push_back({10, 12});

    return snake_pos;
}

void process_input(int userInput, int& dirY, int& dirX)
{
    switch(userInput)
    {
        case 'w':
        case KEY_UP:

            if (dirY != 1)
            {
                dirY = -1;
                dirX = 0;
            }

            break;

        case 's':
        case KEY_DOWN:

            if (dirY != -1)
            {
                dirY = 1;
                dirX = 0;
            }

            break;

        case 'a':
        case KEY_LEFT:

            if (dirX != 1)
            {
                dirY = 0;
                dirX = -1;
            }

            break;

        case 'd':
        case KEY_RIGHT:

            if (dirX != -1)
            {
                dirY = 0;
                dirX = 1;
            }

            break;
    }
}

void move_snake(vector<pair<int, int>>& snake_pos,
                vector<Item>& items,
                int& dirY,
                int& dirX)
{
    int newY = snake_pos[0].first + dirY;

    int newX = snake_pos[0].second + dirX;

    bool grow = false;

    bool poison = false;

    for (int i = 0; i < items.size(); i++)
    {
        if (items[i].y == newY &&
            items[i].x == newX)
        {
            if (items[i].type == GROWTH_ITEM)
            {
                grow = true;
            }

            else if (items[i].type == POISON_ITEM)
            {
                poison = true;
            }

            items.erase(items.begin() + i);

            break;
        }
    }

    snake_pos.insert(snake_pos.begin(), {newY, newX});

    if (!grow)
    {
        snake_pos.pop_back();
    }

    if (poison && snake_pos.size() > 1)
    {
        snake_pos.pop_back();
    }
}