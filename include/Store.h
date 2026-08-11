#ifndef STORE_H
#define STORE_H
#include "Player.h"
#include "Equippable.h"
#include "Item.h"


class Store {
private:
    Item** storeInventory;      // holds all items being sold
    int numStoreItems;
    int capacity;
    void sortStoreInventory();
    void grow();
public:
    Store(int cap = 10);
    ~Store();
    
    void stockItem(Item* item);

    void buy(Player* player, int itemIndex);
    void sell(Player* player, int itemIndex);
    // void repair(Player* player, Equippable* equipment);
    void openInterface(Player* player);


};


#endif