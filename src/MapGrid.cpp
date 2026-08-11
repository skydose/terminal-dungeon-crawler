#include "MapGrid.h"
#include <iostream>
#include "Player.h"

#include "json.hpp"
#include "EmptyRoom.h"
#include "SaveRoom.h"
#include "CombatRoom.h"
#include "Goblin.h"
#include "Knight.h"
#include "Boss.h"
#include "colorcodes.h"
#include <cstdlib>

using namespace std;


MapGrid::MapGrid(int w, int h) : width(w), height(h), rooms(w, h) {
    playerPosition = {0, 0};
}

MapGrid::~MapGrid() {
    clearGrid(); 
}


// picking up every room individually then deleting it
void MapGrid::clearGrid() {
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            Room* current = rooms.get(i, j);
            if (current != nullptr) {
                delete current;
                rooms.set(i, j, nullptr); 
            }
        }
    }
}

void MapGrid::setRoom(int x, int y, Room* room) {
    rooms.set(x, y, room); 
}

Coordinate MapGrid::getPlayerPosition() const { return playerPosition; }

bool MapGrid::move(int dx, int dy, Player* player) {
    // dx and dy are usually just +1 or -1, they will add to current pos
    int newX = playerPosition.x + dx;
    int newY = playerPosition.y + dy;

    // first checks bounds
    if(newX < 0 || newX >= width || newY < 0 || newY >= height) {
        cout << "You cannot go out of bounds!\n";
        return false;
    }

    // checks the room in that future position
    Room* nextRoom = rooms.get(newX, newY); 

    // if there ISNT a room there then you just dont enter and pretend theres a locked door there
    if(nextRoom == nullptr) {
        cout << ">> Hmm... the door doesn't seem to open.\n";
        return false;
    }

    // we make a current room variable that stores the current room of the player
    Room* currentRoom = rooms.get(playerPosition.x, playerPosition.y);
    if (currentRoom != nullptr) {
        CombatRoom* combatRoom = dynamic_cast<CombatRoom*>(currentRoom);
        
        // if the current room NOT empty AND isnt cleared
        if (combatRoom != nullptr && !combatRoom->getIsCleared()) {
            // if the current room is NOT cleared then we cannot go to the nextRoom because there used to be a glitch where i could skip rooms
            if (!nextRoom->getIsCleared()) {
                cout << RED << ">> The enemies block the exit! You must defeat them to proceed further.\n" << RESET;
                return false;
            }
        }
    }

    // if we pass the lock check, move the player
    playerPosition.x = newX;
    playerPosition.y = newY;

    // a bunccch of checks just to move :')


    nextRoom->enter(player);
    nextRoom->interact(player);
    
    return true;
}

void MapGrid::render() {
    // pretty formatting below

    cout << "\033[2J\033[1;1H";         // ansi new space

    cout << CYAN << "============================================================\n" << RESET;
    cout << CYAN << "||" << RESET << "                   DUNGEON MAP                          " << CYAN << "||\n" << RESET;
    cout << CYAN << "============================================================\n" << RESET;
    cout << CYAN << "||                                                        ||\n" << RESET;

    for (int y = 0; y < height; ++y) {
        cout << CYAN << "||   " << RESET; 
        
        for (int x = 0; x < width; ++x) {
            if (playerPosition.x == x && playerPosition.y == y) {
                cout << "[" << GREEN << "P" << RESET << "]  "; 
            } 
            else if (rooms.get(x, y) != nullptr) {
                
                if (x == width - 1 && y == height - 1) {
                    if (rooms.get(x, y)->getIsCleared()) {
                        cout << "[" << GRAY << "_" << RESET << "]  ";
                    } else {
                        cout << "[" << RED << "B" << RESET << "]  ";
                    }
                }
                else if (rooms.get(x, y)->getIsCleared()) {
                    cout << "[" << GRAY << "_" << RESET << "]  ";
                } else {
                    cout << "[" << YELLOW << "?" << RESET << "]  ";
                }
                
            } 
            else {
                cout << GRAY << "  .  " << RESET;
            }
        }
        cout << CYAN << "   ||\n" << RESET; 
    }

    cout << CYAN << "||                                                        ||\n" << RESET;
    cout << CYAN << "============================================================\n" << RESET;
    
    cout << " Legend:\n";
    cout << " [" << GREEN << "P" << RESET << "] You    |  "
         << "[" << RED << "B" << RESET << "] Boss    |  "
         << "[" << YELLOW << "?" << RESET << "] Unknown\n"
         << " [" << GRAY << "_" << RESET << "] Cleared|  "
         << GRAY << ".   Wall\n" << RESET;
                
    cout << CYAN << "------------------------------------------------------------\n" << RESET;
    cout << " Navigation: W (Up), A (Left), S (Down), D (Right)\n";
    cout << CYAN << "============================================================\n" << RESET;
}

void MapGrid::serialize(json& j) const {
    j["width"] = width;
    j["height"] = height;
    j["player_x"] = playerPosition.x;
    j["player_y"] = playerPosition.y;

    json roomsArray = json::array();

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (rooms.get(x, y) != nullptr) {
                
                json roomJson; 
                
                rooms.get(x, y)->serialize(roomJson);
                
                roomJson["x"] = x; 
                roomJson["y"] = y;
                roomsArray.push_back(roomJson);
            }
        }
    }
    
    j["rooms"] = roomsArray;
}

void MapGrid::deserialize(const json& j) {
    clearGrid();    // helper function to delete current rooms and prevent memory leaks

    playerPosition.x = j.value("player_x", 0);
    playerPosition.y = j.value("player_y", 0);

    for (const auto& roomData : j["rooms"]) {
        int x = roomData.value("x", 0);
        int y = roomData.value("y", 0);
        string type = roomData.value("type", "EmptyRoom");
        string narrative = roomData.value("narrative", "");

        Room* newRoom = nullptr;

        if (type == "EmptyRoom") {
            newRoom = new EmptyRoom(narrative);
        } 
        else if (type == "SaveRoom") {
            newRoom = new SaveRoom(narrative, new Store(10)); 
        } 
        else if (type == "CombatRoom") {
            int totalEnemies = roomData.value("numEnemies", 0);
            
            Enemy** loadedEnemies = nullptr;
            
            if (totalEnemies > 0 && roomData.contains("enemies")) {
                loadedEnemies = new Enemy*[totalEnemies];
                int i = 0;
                
                for (const auto& enemyJson : roomData["enemies"]) {
                    string enemyType = enemyJson.value("type", "Dead");
                    
                    if (enemyType == "Goblin") loadedEnemies[i] = new Goblin();
                    else if (enemyType == "Knight") loadedEnemies[i] = new Knight();
                    else if (enemyType == "Boss") loadedEnemies[i] = new Boss();
                    else loadedEnemies[i] = nullptr; 
                    
                    if (loadedEnemies[i] != nullptr) {
                        loadedEnemies[i]->deserialize(enemyJson);
                    }
                    i++;
                }
            }
            
            newRoom = new CombatRoom(narrative, loadedEnemies, totalEnemies);
        }

        if (newRoom != nullptr) {
            newRoom->deserialize(roomData);
            rooms.set(x, y, newRoom);
        }
    }
}