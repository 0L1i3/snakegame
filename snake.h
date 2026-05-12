#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <utility>

#include "item.h"

using namespace std;

vector<pair<int, int>> place_snake();

void process_input(int userInput, int& dirY, int& dirX);

void move_snake(vector<pair<int, int>>& snake_pos,
                vector<Item>& items,
                int& dirY,
                int& dirX);

#endif