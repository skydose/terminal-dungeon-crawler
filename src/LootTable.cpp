#include "LootTable.h"
#include "json.hpp"

using json = nlohmann::json;



LootBundle::LootBundle() {
    xp = 0;
    gold = 0;
    capacity = 5;
}

LootBundle::LootBundle(int xp, int gold, int capacity) {
    this->xp = (xp < 0) ? 0 : xp;
    this->gold = (gold < 0) ? 0 : gold;
    this->capacity = (capacity <= 0) ? 1 : capacity;
}

LootBundle::LootBundle(int xp, int gold, DynamicArray<Item*> items) {
    this->xp = xp;
    this->gold = gold;
    this->items = items;
    this->capacity = items.size();
}


LootBundle::LootBundle(const LootBundle& other) {
    items = other.items;
    capacity = other.capacity;
    xp = other.xp;
    gold = other.gold;
}

LootBundle& LootBundle::operator=(const LootBundle& other) {
    if (this == &other) {
        return *this;
    }
    items = other.items;
    capacity = other.capacity;
    xp = other.xp;
    gold = other.gold;
    return *this;
}

LootBundle::~LootBundle() {}

int LootBundle::getCapacity() const {
    return capacity;
}

Item* LootBundle::getItem(int index) const {
    if (index < 0 || index >= items.size()) {
        return nullptr;
    }

    return items[index];
}

bool LootBundle::addItem(Item* item) {
    if (item == nullptr) {
        return false;
    }
    items.push_back(item);
    return true;
}

LootBundle& LootBundle::operator+=(const LootBundle& other) {
    xp += other.xp;
    gold += other.gold;
    for (int i = 0; i < other.items.size(); i++) {
        addItem(other.items[i]);
    }
    return *this;
}

void LootBundle::saveToJson(const std::string& fileName) const {
    json j;

    j["module"] = "LootBundle";
    j["xp"] = xp;
    j["gold"] = gold;
    j["items.size()"] = items.size();

    j["items"] = json::array();

    for (int i = 0; i < items.size(); i++) {
        json itemJson;
        itemJson["slot"] = i;

        if (items[i] != nullptr) {
            itemJson["itemName"] = items[i]->getName();
        }
        else {
            itemJson["itemName"] = "NULL";
        }
        j["items"].push_back(itemJson);
    }

    std::ofstream out(fileName);

    if (!out) {
        std::cout << "Could not create loot bundle JSON file.\n";
        return;
    }

    out << j.dump(4);
    out.close();

    std::cout << "Loot bundle saved to " << fileName << "\n";
}

std::ostream& operator<<(std::ostream& out, const LootBundle& bundle) {
    out << "XP: " << bundle.xp
        << ", Gold: " << bundle.gold
        << ", Items Dropped: " << bundle.items.size();

    return out;
}



DropEntry::DropEntry(Item* item, float chance) {
    this->item = item;
    setChance(chance);
}

void DropEntry::setItem(Item* item) {
    this->item = item;
}

void DropEntry::setChance(float chance) {
    this->chance = (chance > 1.0f)? 1.0f : (chance < 0.0f)? 0.0f : chance;
}


LootTable::LootTable() {
    capacity = 5;
    numEntries = 0;

    minXP = 10;
    maxXP = 30;
    minGold = 5;
    maxGold = 20;

    entries = new DropEntry[capacity];
}

LootTable::LootTable(int capacity) {
    this->capacity = (capacity <= 0) ? 1 : capacity;
    numEntries = 0;

    minXP = 10;
    maxXP = 30;
    minGold = 5;
    maxGold = 20;

    entries = new DropEntry[this->capacity];
}

void LootTable::copyFrom(const LootTable& other) {
    capacity = other.capacity;
    numEntries = other.numEntries;

    minXP = other.minXP;
    maxXP = other.maxXP;
    minGold = other.minGold;
    maxGold = other.maxGold;
    entries = new DropEntry[capacity];

    for (int i = 0; i < numEntries; i++) {
        entries[i] = other.entries[i];
    }
}


LootTable::LootTable(const LootTable& other) {
    copyFrom(other);
}

LootTable& LootTable::operator=(const LootTable& other) {
    if (this != &other) {
        delete[] entries;
        copyFrom(other);
    }

    return *this;
}

LootTable::~LootTable() {
    delete[] entries;
}

int LootTable::getNumEntries() const {
    return numEntries;
}

int LootTable::getCapacity() const {
    return capacity;
}

void LootTable::resize() {
    int newCapacity = capacity * 2;

    DropEntry* newEntries = new DropEntry[newCapacity];

    for (int i = 0; i < numEntries; i++) {
        newEntries[i] = entries[i];
    }

    delete[] entries;
    entries = newEntries;
    capacity = newCapacity;
}

void LootTable::setXPRange(int minXP, int maxXP) {
    if (minXP < 0) {
        minXP = 0;
    }
    if (maxXP < minXP) {
        maxXP = minXP;
    }
    this->minXP = minXP;
    this->maxXP = maxXP;
}

void LootTable::setGoldRange(int minGold, int maxGold) {
    if (minGold < 0) {
        minGold = 0;
    }
    if (maxGold < minGold) {
        maxGold = minGold;
    }
    this->minGold = minGold;
    this->maxGold = maxGold;
}

bool LootTable::addEntry(Item* item, float chance) {
    if (item == nullptr) {
        std::cout << "Cannot add empty item to loot table.\n";
        return false;
    }

    if (chance <= 0.0f) {
        std::cout << "Drop chance must be greater than 0.\n";
        return false;
    }

    if (numEntries >= capacity) {
        resize();
    }

    entries[numEntries] = DropEntry(item, chance);
    numEntries++;

    return true;
}

LootBundle LootTable::roll() const {
    int xpReward = minXP;

    if (maxXP > minXP) {
        xpReward = minXP + rand() % (maxXP - minXP + 1);
    }
    int goldReward = minGold;

    if (maxGold > minGold) {
        goldReward = minGold + rand() % (maxGold - minGold + 1);
    }
    LootBundle bundle(xpReward, goldReward, numEntries);


    for (int i = 0; i < numEntries; i++) {
        float randomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

        if (randomValue <= entries[i].chance) {
            bundle.addItem(entries[i].item);
        }
    }

    std::cout << "Enemy dropped " << bundle.xp
             << " XP and " << bundle.gold << " Gold.\n";

    return bundle;
}
LootBundle LootTable::rollForLoot() const {
    return roll();
}

void LootTable::saveToJson(const std::string& fileName) const {
    json j;

    j["module"] = "LootTable";
    j["numEntries"] = numEntries;
    j["capacity"] = capacity;

    j["xpRange"]["min"] = minXP;
    j["xpRange"]["max"] = maxXP;

    j["goldRange"]["min"] = minGold;
    j["goldRange"]["max"] = maxGold;

    j["dropEntries"] = json::array();

    for (int i = 0; i < numEntries; i++) {
        json entryJson;
        entryJson["entryId"] = i;
        entryJson["chance"] = entries[i].chance;
        if (entries[i].item != nullptr) {
            entryJson["itemName"] = entries[i].item->getName();
        }

        else {
            entryJson["itemName"] = "NULL";
        }
        j["dropEntries"].push_back(entryJson);
    }
    std::ofstream out(fileName);

    if (!out) {
        std::cout << "Could not create loot table JSON file.\n";
        return;
    }
    out << j.dump(4);
    out.close();

    std::cout << "Loot table saved to " << fileName << "\n";
}