#include "item.h"

void create_items(vector<Item>& items){
    items.clear();

    items.push_back({5,5,GROWTH_ITEM});

    items.push_back({10,10,POISON_ITEM});
}
void apply_items(int map_data[21][21], vector<Item>& items){
    for(auto& item : items){
        map_data[item.y][item.x] = item.type;
    }
}