#ifndef ITEM_H
#define ITEM_H

#include <vector>
using namespace std;

#define GROWTH_ITEM 5
#define POISON_ITEM 6

struct Item{
    int y;
    int x;
    int type;
};


void create_items(vector<Item>& items);
void apply_items(int map_data[21][21], vector<Item>& items);

#endif
