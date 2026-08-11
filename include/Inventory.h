#ifndef INVENTORY_H
#define INVENTORY_H

#include "DynamicArray.h"
#include "Serializable.h" 
#include "Item.h"

class Inventory : public Serializable {
private:

// using bsce25019's dynamic array template class to ease the workload for the inventory
    DynamicArray<Item*> items;
    int capacity;
public:
    Inventory(int cap);
    virtual ~Inventory();

    bool canFit(Item*) const;
    bool addItem(Item* item);
    void removeItem(int index);
    Inventory& operator+=(Item* item);
    friend std::ostream& operator<<(std::ostream& os, const Inventory& inv);
    
    Item* getItem(int index) const;
    int getNumItems() const { return items.size(); }
    int getCapacity() const { return capacity; }
    void display() const;
    void serialize(json& j) const override;
    void deserialize(const json& j) override;
};

#endif