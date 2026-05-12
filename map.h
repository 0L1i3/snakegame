#ifndef MAP_H
#define MAP_H

#include <ncurses.h>
#include <vector>

#include "item.h"

#define MAP_SIZE 21

using namespace std;

extern int original_map_data[MAP_SIZE][MAP_SIZE];

void set_color();

void fill_board(WINDOW* win,
                vector<pair<int, int>>& snake_pos,
                vector<Item>& items);

#endif