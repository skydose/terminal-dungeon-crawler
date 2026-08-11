#include "Inventory.h"
#include "Item.h"
#include "Weapon.h"
#include "Armor.h"
#include "Consumable.h"

#include "colorcodes.h" 
#include <iostream>

using namespace std;

Inventory::Inventory(int cap) : capacity(cap) {}

Inventory::~Inventory() {
    for (int i = 0; i < items.size(); ++i) {
        delete items[i]; 
    }
}

bool Inventory::canFit(Item*) const {
    return items.size() < capacity;
}

bool Inventory::addItem(Item* item) {
    if (canFit(item) && item != nullptr) {
        items.push_back(item);
        return true;
    }
    return false;
}

void Inventory::removeItem(int index) {
    items.removeAt(index);
}

Item* Inventory::getItem(int index) const {
    if (index >= 0 && index < items.size()) {
        return items[index];
    }
    return nullptr;
}


void Inventory::display() const {
    // the display looks so cool ngl

    cout << "\033[2J\033[1;1H"; // ANSI Clear Screen
    
    cout << CYAN << "╔═══════════════════════════════════════╗\n" << RESET;
    cout << CYAN << "║" << RESET << "          " << YELLOW << "ADVENTURER'S INVENTORY" << RESET << "         " << CYAN << "║\n" << RESET;
    cout << CYAN << "╠═══════════════════════════════════════╣\n" << RESET;
    
    // Capacity Tracker
    cout << CYAN << "║" << RESET << "   Capacity: [" << items.size() << " / " << capacity << "]                  " << CYAN << "║\n" << RESET;
    cout << CYAN << "╠═══════════════════════════════════════╣\n" << RESET;

    for (int i = 0; i < capacity; ++i) {
        cout << CYAN << "║ " << RESET;
        
        if (i + 1 < 10) cout << " ";
        
        if (i < items.size() && items[i] != nullptr) {
            cout << YELLOW << "[" << (i + 1) << "] " << RESET;
            
            string itemName = items[i]->getName();
            cout << itemName;
            
            int padding = 30 - itemName.length();
            for(int p = 0; p < padding; p++) {
                cout << " ";
            }
        } else {
            cout << GRAY << "[" << (i + 1) << "] --- Empty Slot ---          " << RESET;
        }
        cout << CYAN << " ║\n" << RESET;
    }
    cout << CYAN << "╚═══════════════════════════════════════╝\n" << RESET;
}


// some operator overloading even though didnt need em
Inventory& Inventory::operator+=(Item* item) {
    this->addItem(item); 
    return *this;
}

ostream& operator<<(std::ostream& os, const Inventory& inv) {
    os << "Inventory Status: " << inv.getNumItems() << "/" << inv.getCapacity() << " slots filled.";
    return os;
}



void Inventory::serialize(json& j) const {
    j["capacity"] = capacity;
    
    json itemsArray = json::array();
    
    for (int i = 0; i < items.size(); ++i) {
        Item* item = items[i];
        if (item != nullptr) {
            json itemJson;
            item->serialize(itemJson); 
            itemsArray.push_back(itemJson);
        }
    }
    
    j["items"] = itemsArray;
}

void Inventory::deserialize(const json& j) {
    for (int i = 0; i < items.size(); ++i) {
        delete items[i];
    }
    while (items.size() > 0) {
        items.removeAt(0); 
    }

    capacity = j.value("capacity", 10);

    if (j.contains("items")) {
        for (const auto& itemJson : j["items"]) {
            std::string type = itemJson.value("type", "Unknown");
            Item* newItem = nullptr;

            if (type == "Sword") newItem = new Sword();
            else if (type == "Axe") newItem = new Axe();
            else if (type == "Dagger") newItem = new Dagger();
            else if (type == "HealthPotion") newItem = new HealthPotion();
            else if (type == "DamagePotion") newItem = new DamagePotion();
            else if (type == "ManaPotion") newItem = new ManaPotion();
            else if (type == "ChainMail") newItem = new ChainMail();
            else if (type == "PlateArmor") newItem = new PlateArmor();

            if (newItem != nullptr) {
                newItem->deserialize(itemJson);
                addItem(newItem);
            }
        }
    }
}