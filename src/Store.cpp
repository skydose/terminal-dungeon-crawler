#include "Store.h"
#include "Inventory.h"
#include <iostream>
#include "colorcodes.h"

#include "Spell.h"
#include "Spellbook.h"

#include "InputManager.h"
using namespace std;

void Store::grow() {
    int newCapacity = capacity * 2;
    Item** temp = new Item*[newCapacity];
    
    for(int i = 0; i < capacity; i++) {
        temp[i] = storeInventory[i];
    }
    for(int i = capacity; i < newCapacity; i++) {
        temp[i] = nullptr;
    }
    
    delete[] storeInventory;
    storeInventory = temp;
    capacity = newCapacity;
}

Store::Store(int cap) : numStoreItems(0), capacity(cap) {
    storeInventory = new Item*[capacity];
    for(int i = 0; i < capacity; i++) {
        storeInventory[i] = nullptr;
    }
}
Store::~Store() {
    if(storeInventory != nullptr) {
        for(int i = 0; i < capacity; i++) {
            if(storeInventory[i] != nullptr) delete storeInventory[i];
        }
        delete[] storeInventory;
        storeInventory = nullptr;
    }
}
    
void Store::stockItem(Item* item) {
    if(numStoreItems < capacity && item != nullptr) {
        storeInventory[numStoreItems++] = item;
    }
}

void Store::sortStoreInventory() {
    int newCount = 0;
    for(int i = 0; i < capacity; i++) {
        if(storeInventory[i] != nullptr) {
            storeInventory[newCount++] = storeInventory[i];
        }
    }
    for(int i = newCount; i < capacity; i++) {
        storeInventory[i] = nullptr;
    }
    numStoreItems = newCount;
}
void Store::buy(Player* player, int itemIndex) {
    if(itemIndex < 0 || itemIndex >= numStoreItems || storeInventory[itemIndex] == nullptr) {
        cout << "Invalid item!\n";
        return;
    }
    Item* selectedItem = storeInventory[itemIndex];
    int cost = selectedItem->getValue();

    if(player->getGold() < cost) {
        cout << "Not enough gold! You need " << cost << " gold.\n";
        return;
    }

    Inventory* inventory = player->getInventory();
    if(inventory != nullptr && inventory->canFit(selectedItem)) {
        player->deductGold(cost);
        inventory->addItem(selectedItem);

        cout << "Bought " << selectedItem->getName() << " for " << cost << " gold!\n";

        storeInventory[itemIndex] = nullptr;
        sortStoreInventory();
    }
    else cout << "Your inventory is full!\n";
}
void Store::sell(Player* player, int itemIndex) {
    Inventory* inventory = player->getInventory();

    if(inventory == nullptr) return;

    Item* sellItem = inventory->getItem(itemIndex);
    if(sellItem == nullptr) { 
        cout << "Invalid item!\n";
        return;
    }
    if(numStoreItems >= capacity) grow();

    int sellPrice = sellItem->getValue() * 0.5f;

    player->addGold(sellPrice);
    inventory->removeItem(itemIndex);

    storeInventory[numStoreItems++] = sellItem;

    cout << "Sold " << sellItem->getName() << " for " << sellPrice << " gold!\n";
}


// void Store::repair(Player* player, Equippable* equipment) {}


void Store::openInterface(Player* player) {
    // i thought it would be interesting to include re4 quotes
    const string merchantLines[] = {
        "Let's do some business then, eh!",
        "You have the stench of battle on you, mate. Heh heh.",
        "I've got some new items in stock, heh heh. Come, take a look!",
        "Heh heh. Having a rough day, stranger?",
        "I reserved some items, just for you! My other customer doesn't know about these, heh heh heh.",
        "Welcome!",
        "Got something new for ya.",
        "Oh, what're ya buyin'?",
        "What're ya sellin'?",
        "Well then. What can I do you for?",
        "What you see is what I've got."
    };

    // it picks a random line from the above
    int randomIndex = rand() % 11;
    string currentGreeting = merchantLines[randomIndex];


    bool shopping = true;
    string actionMessage = "";

    while (shopping) {
        cout << "\033[2J\033[1;1H";
        
        cout << "\n" << MAGENTA << "Merchant: " << RESET << BOLD << "\"" << currentGreeting << "\"\n\n" << RESET;
        cout << CYAN << "╔═══════════════════════════════════════╗\n" << RESET;
        cout << CYAN << "║" << RESET << YELLOW << "             MERCHANT'S WARES          " << CYAN << "║\n" << RESET;
        cout << CYAN << "╠═══════════════════════════════════════╣\n" << RESET;
        
        string goldStr = to_string(player->getGold()) + " G";
        cout << CYAN << "║" << RESET << "  Your Gold: [" << YELLOW << goldStr << RESET << "]";
        int goldPad = 23 - goldStr.length();
        for(int p = 0; p < goldPad; p++) cout << " ";
        cout << CYAN << "║\n" << RESET;
        
        cout << CYAN << "╠═══════════════════════════════════════╣\n" << RESET;
        cout << CYAN << "║ " << RESET << "[1] Buy Items                         " << CYAN << "║\n" << RESET;
        cout << CYAN << "║ " << RESET << "[2] Sell Items                        " << CYAN << "║\n" << RESET;
        
        bool canBuyBook = (player->getSpellbook() == nullptr);
        
        if (canBuyBook) {
            cout << CYAN << "║ " << RESET << MAGENTA << "[3] Buy A Spellbook (100 G)       " << CYAN << "║\n" << RESET;
            cout << CYAN << "║ " << RESET << "[4] Leave Store                       " << CYAN << "║\n" << RESET;
        } else {
            cout << CYAN << "║ " << RESET << "[3] Leave Store                       " << CYAN << "║\n" << RESET;
        }
        cout << CYAN << "╚═══════════════════════════════════════╝\n" << RESET;

        if (actionMessage != "") {
            cout << "\n> " << actionMessage << "\n";
            actionMessage = "";
        }

        cout << "\nChoice: ";
        char choice = InputManager::getChar();

        if (choice == '1') {
            cout << "\033[2J\033[1;1H"; 
            cout << CYAN << "╔═══════════════════════════════════════╗\n" << RESET;
            cout << CYAN << "║" << RESET << YELLOW << "            STORE INVENTORY            " << CYAN << "║\n" << RESET;
            cout << CYAN << "╠═══════════════════════════════════════╣\n" << RESET;
            
            if (numStoreItems == 0) {
                cout << CYAN << "║" << RESET << GRAY << "       The merchant is sold out!       " << CYAN << "║\n" << RESET;
            } else {
                for (int i = 0; i < numStoreItems; ++i) {
                    cout << CYAN << "║ " << RESET;
                    if (i + 1 < 10) cout << " ";
                    cout << YELLOW << "[" << (i + 1) << "] " << RESET; // 1-based indexing
                    
                    string itemName = storeInventory[i]->getName();
                    string costStr = "(" + to_string(storeInventory[i]->getValue()) + "g)";
                    
                    cout << itemName;
                    
                    // Math to perfectly align the prices to the right wall
                    int padding = 32 - itemName.length() - costStr.length();
                    for(int p = 0; p < padding; p++) cout << " ";
                    
                    cout << GREEN << costStr << RESET << CYAN << " ║\n" << RESET;
                }
            }
            cout << CYAN << "╚═══════════════════════════════════════╝\n" << RESET;
            cout << "\nEnter item number to buy (or 0 to cancel): ";
            
            int buyIdx;
            cin >> buyIdx;
            if (cin.fail()) {
                cin.clear(); cin.ignore(10000, '\n'); continue;
            }
            cin.ignore(10000, '\n');

            if (buyIdx > 0 && buyIdx <= numStoreItems) {
                buy(player, buyIdx - 1); // subtract 1 for array indexing
                cout << "\nPress Enter to continue...";
                cin.get(); 
            }
        } 
        
        else if (choice == '2') {
            player->getInventory()->display(); 
            cout << "\nEnter item number to sell (or 0 to cancel): ";
            
            int sellIdx;
            cin >> sellIdx;
            if (cin.fail()) {
                cin.clear(); cin.ignore(10000, '\n'); continue;
            }
            cin.ignore(10000, '\n');

            if (sellIdx > 0 && sellIdx <= player->getInventory()->getNumItems()) {
                sell(player, sellIdx - 1);
                cout << "\nPress Enter to continue...";
                cin.get();
            }
        } 
        
        else if (choice == '3' && canBuyBook) {
            if (player->getGold() >= 100) {
                player->deductGold(100);
                
                Spellbook* sb = new Spellbook();
                
                sb->addSpell(new DamageSpell("Arcane Dart", 10, 25)); 
                
                if (player->getLevel() >= 2) sb->addSpell(new HealingSpell("Lesser Heal", 20, 40));
                if (player->getLevel() >= 3) sb->addSpell(new DamageSpell("Fireball", 30, 50));
                if (player->getLevel() >= 4) sb->addSpell(new HealingSpell("Great Heal", 40, 100));
                if (player->getLevel() >= 5) sb->addSpell(new DamageSpell("Lightning Storm", 50, 80));
                
                player->setSpellbook(sb);
                
                cout << MAGENTA << "\n>>> You paid 100 G and unlocked the Mystic Grimoire!\n" << RESET;
                cout << MAGENTA << ">>> Spells up to Level " << player->getLevel() << " are now available in combat!\n" << RESET;
            } else {
                cout << RED << "\nThe merchant shakes their head. You need 100 Gold to afford that.\n" << RESET;
            }
            cout << "\nPress Enter to continue...";
            cin.get();
        } 
        
        else if ((choice == '3' && !canBuyBook) || choice == '4') {
            shopping = false;
        } 
        
        else {
            actionMessage = "Invalid choice.";
        }
    }
}