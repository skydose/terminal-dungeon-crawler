#include "CombatRoom.h"
#include <iostream>
#include "BattleManager.h"
#include "LootTable.h"
#include "Inventory.h"
#include "colorcodes.h"
#include "InputManager.h"
#include "Enemy.h"
#include "Player.h"

CombatRoom::CombatRoom(string text, Enemy** enemyGroup, int groupSize) : Room(text, "CombatRoom"), enemies(enemyGroup), numEnemies(groupSize), floorCapacity(15), numFloorItems(0) {
    
    floorLoot = new Item*[floorCapacity];
    for(int i = 0; i < floorCapacity; i++) {
        floorLoot[i] = nullptr;
    }

    roomWidth = 15;
    roomHeight = 7;
    // door at bottom cetner
    doorPos = {roomWidth / 2, roomHeight - 1}; 
}


CombatRoom::~CombatRoom() {
    for(int i = 0; i < numEnemies; i++) {
        if(enemies[i] != nullptr) {
            delete enemies[i];
            enemies[i] = nullptr;
        }
    }

    if(enemies != nullptr) {
        delete[] enemies;
        enemies = nullptr;
    }

    if (floorLoot != nullptr) {
            for (int i = 0; i < floorCapacity; ++i) {
                if (floorLoot[i] != nullptr) delete floorLoot[i];
            }
            delete[] floorLoot;
        }
}


// pretty ui formatting >:D
void CombatRoom::enter(Player* player) {
    Room::enter(player);        // calling the base class function too because it has the room intro
    if(!isCleared) cout << ">> You have " << numEnemies << " blocking your path!\n";
}

void CombatRoom::render() {
    cout << "\033[2J\033[1;1H"; // ANSI clear screen

    cout << RED << "    =======================================\n" << RESET;
    if (isCleared) {
        cout << GRAY << "   ||" << RESET << "            CLEARED ROOM             " << GRAY << "||\n" << RESET;
    } else {
        cout << RED << "    ||" << RESET << "             COMBAT ZONE             " << RED << "||\n" << RESET;
    }
    cout << RED << "    =======================================\n" << RESET;

    for (int y = 0; y < roomHeight; ++y) {
        cout << "  "; 
        for (int x = 0; x < roomWidth; ++x) {
            if (x == 0 || x == roomWidth - 1 || y == 0 || y == roomHeight - 1) {
                if (x == doorPos.x && y == doorPos.y) cout << CYAN << "[D]" << RESET;
                else cout << RED << " # " << RESET;
            } 
            else if (x == localPlayer.x && y == localPlayer.y) {
                cout << GREEN << "[P]" << RESET;
            } 
            // showing loot on the floor!
            else if (isCleared && numFloorItems > 0 && x == roomWidth/2 && y == roomHeight/2) {
                cout << YELLOW << "[$]" << RESET;
            }
            // representing the enemies
            else if (!isCleared && y == 2) { 
                int startX = (roomWidth / 2) - (numEnemies / 2);
                int enemyIndex = x - startX;
                if (enemyIndex >= 0 && enemyIndex < numEnemies && enemies[enemyIndex] != nullptr && !enemies[enemyIndex]->isDead()) {
                    cout << MAGENTA << "[E]" << RESET; 
                } else {
                    cout << " . "; 
                }
            } else {
                cout << " . "; 
            }
        }
        cout << "\n";
    }
    cout << "\n Legend: " << GREEN << "[P]" << RESET << " You | " 
         << MAGENTA << "[E]" << RESET << " Hostile | " 
         << YELLOW << "[$]" << RESET << " Loot | "
         << CYAN << "[D]" << RESET << " Door\n";
}


void CombatRoom::interact(Player* player) {
    if (!player) return;

    localPlayer = {doorPos.x, doorPos.y - 1}; 
    bool inRoom = true;
    string actionMessage = narrativeText; 

    while (inRoom && !player->isDead()) {
        render();
        
        if (actionMessage != "") {
            cout << "\n > " << actionMessage << "\n";
            actionMessage = ""; 
        }

        static bool firstCombat = true;      // this will change once and will remain changed for every upcoming call
        if(firstCombat) {
            cout << "\n" << BLUE << "You: " << RESET << BOLD << " \"Well- who would've guessed? Monsters in a sketchy dungeon?\"\n" << RESET;
            InputManager::pause();
            firstCombat = false;
            render();
        }

        if (!isCleared) cout << RED << "\n Hostiles detected! Press 'W' to initiate combat.\n" << RESET;
        else if (numFloorItems > 0) cout << YELLOW << "\n Items are scattered on the floor.\n" << RESET;

        cout << "\n Action (W/A/S/D to move, Q to escape): ";
        
        // this uses the keystroke header i put
        char input = InputManager::getChar();

        int dx = 0, dy = 0;
     // just movement mechanics here, WASD translates to these chanes in coordinates
        switch (input) {
            case 'w': case 'W': dy = -1; break;
            case 's': case 'S': dy = 1; break;
            case 'a': case 'A': dx = -1; break;
            case 'd': case 'D': dx = 1; break;
            case 'q': case 'Q': inRoom = false; continue; 
            default: continue;
        }

        // stores the next position
        Coordinate nextPos = {localPlayer.x + dx, localPlayer.y + dy};

        if (isCleared && numFloorItems > 0 && nextPos.x == roomWidth/2 && nextPos.y == roomHeight/2) {
            collectFloorLoot(player);
            actionMessage = "You scoured the area for items.";
            localPlayer = nextPos;
            continue;
        }

        if (!isCleared && dy == -1) { 
            cout << "\n\n*** ATTACK INITIATED ***\n";
            
            DynamicArray<Enemy*> battleSquad;
            int activeEnemyIndex = -1;
            
            // FIND ONLY THE FIRST LIVING ENEMY (because its turn based and you fight one by one)
            for (int i = 0; i < numEnemies; i++) {
                // checks for alive enemies in the room and puts em in battleSquad
                if (enemies[i] != nullptr && !enemies[i]->isDead()) {
                    battleSquad.push_back(enemies[i]);
                    activeEnemyIndex = i;
                    break; 
                }
            }

            // start the 1v1 Battle
            BattleManager::getInstance()->startBattle(player, battleSquad);

            // PROCESS POST BATTLE LOOT FOR THIS SPECIFIC ENEMY ONLY
            if (activeEnemyIndex != -1 && enemies[activeEnemyIndex]->isDead()) {
                LootBundle loot = enemies[activeEnemyIndex]->dropLoot();
                player->addXP(loot.xp);
                player->addGold(loot.gold);
                cout << ">> Gained " << loot.xp << " XP and " << loot.gold << " Gold from " << enemies[activeEnemyIndex]->getName() << "!\n";

                for(int j = 0; j < loot.items.size(); j++) {
                    if (player->getInventory()->canFit(loot.items[j])) {
                        player->getInventory()->addItem(loot.items[j]);
                        cout << ">> Looted: " << loot.items[j]->getName() << "\n";
                    } else if (numFloorItems < floorCapacity) {
                        floorLoot[numFloorItems++] = loot.items[j];
                        cout << ">> Inventory full! Dropped " << loot.items[j]->getName() << " on the floor.\n";
                    } else {
                        delete loot.items[j]; 
                    }
                }
                
            }

            // CHECK IF THE ENTIRE ROOM IS CLEARED
            bool allDead = true;
            for (int i = 0; i < numEnemies; ++i) {
                if (enemies[i] != nullptr && !enemies[i]->isDead()) {
                    allDead = false; 
                    break;
                }
            }

            if (allDead) {
                isCleared = true;
                actionMessage = "The room falls dead silent. You are victorious.";
            } else {
                actionMessage = "You defeated one, but more remain in the shadows!";
                localPlayer = {doorPos.x, doorPos.y - 1};
            }
            
            cout << "\nPress Enter to return to the room...";
            cin.ignore(10000, '\n');
            cin.get();
            continue;
        }

        if (nextPos.x == doorPos.x && nextPos.y == doorPos.y) {
            inRoom = false; 
            continue;
        }

        if (nextPos.x > 0 && nextPos.x < roomWidth - 1 && nextPos.y > 0 && nextPos.y < roomHeight - 1) {
            if (!isCleared && nextPos.y <= 3) {
                actionMessage = "The enemies block your path! You must fight!";
            } else {
                localPlayer = nextPos;
            }
        }
    }
}


// helper
void CombatRoom::collectFloorLoot(Player* player) {
    Inventory* inventory = player->getInventory();
    int itemsPicked = 0;

    for(int i = 0; i < numFloorItems; i++) {
        if(floorLoot[i] != nullptr) {

            if(inventory->canFit(floorLoot[i])) {
                inventory->addItem(floorLoot[i]);
                cout << ">> Obtained " << floorLoot[i]->getName() << "!\n";
                floorLoot[i] = nullptr;     // transferring the ownership
                itemsPicked++;
            }
        }
    }

    if(itemsPicked > 0) {
        int newCount = 0;
        for(int i = 0; i < numFloorItems; i++) {
            if(floorLoot[i] != nullptr) {
                floorLoot[newCount++] = floorLoot[i];       // that is to remove the nullptr gaps
            }
        }
     
        // now we make the empty gaps nullptr for efficient memory
        for(int i = newCount; i < numFloorItems; i++) {
            floorLoot[i] = nullptr;
        }
        numFloorItems = newCount;
    }
    if(numFloorItems > 0) {
        cout << "Inventory is full, some items were not picked up!";
    }
}
void CombatRoom::serialize(json& j) const {
    Room::serialize(j); 
    j["type"] = "CombatRoom"; 
    j["numEnemies"] = numEnemies;
    j["isCleared"] = isCleared;

    nlohmann::json enemiesArray = nlohmann::json::array();
    
    for (int i = 0; i < numEnemies; i++) {
        if (enemies != nullptr && enemies[i] != nullptr && !enemies[i]->isDead()) {
            nlohmann::json enemyJson;
            enemies[i]->serialize(enemyJson); 
            enemiesArray.push_back(enemyJson);
        } else {
            // ff the enemy in this slot is dead, save a placeholder
            nlohmann::json deadEnemy;
            deadEnemy["name"] = "Dead"; // matches the MapGrid factory check!
            enemiesArray.push_back(deadEnemy);
        }
    }
    j["enemies"] = enemiesArray;
}

void CombatRoom::deserialize(const nlohmann::json& j) {
    Room::deserialize(j); // automatically loads the narrative text
    
    isCleared = j.value("isCleared", false);
}

// void CombatRoom::serialize(json& j)  {}
// json CombatRoom::deserialize() {}