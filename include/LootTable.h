#ifndef LOOTTABLE_H
#define LOOTTABLE_H
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "DynamicArray.h"
#include "Item.h"


class LootBundle {
private:
    int capacity;
    void resize();
public:
    int xp;
    int gold;
    DynamicArray<Item*> items;

    LootBundle();
    LootBundle(int xp, int gold, int capacity);
    LootBundle(int xp, int gold, DynamicArray<Item*> items);

    LootBundle(const LootBundle& other);
    LootBundle& operator=(const LootBundle& other);
    ~LootBundle();

    int getCapacity() const;
    Item* getItem(int index) const;

    bool addItem(Item* item);

    // Operator overloading to combine loot rewards.
    LootBundle& operator+=(const LootBundle& other);

    void saveToJson(const std::string& fileName) const;
    friend std::ostream& operator<<(std::ostream& out, const LootBundle& bundle);
};

class DropEntry {
public:
    Item* item;
    float chance;

    DropEntry() : item(nullptr), chance(0) {}
    DropEntry(Item* item, float chance);

    void setItem(Item* item);
    void setChance(float chance);
};


class LootTable {
private:
    DropEntry* entries;
    int numEntries;
    int capacity;

    int minXP;
    int maxXP;
    int minGold;
    int maxGold;

    void resize();
    void copyFrom(const LootTable& other);
public:
    LootTable();
    LootTable(int capacity);
    LootTable(const LootTable& other);
    LootTable& operator=(const LootTable& other);
    ~LootTable();

    int getNumEntries() const;
    int getCapacity() const;

    void setXPRange(int minXP, int maxXP);
    void setGoldRange(int minGold, int maxGold);

    bool addEntry(Item* item, float chance);

    LootBundle roll() const;
    LootBundle rollForLoot() const;

    void saveToJson(const std::string& fileName) const;
};


#endif

