#include "SaveRoom.h"
#include <iostream>
#include "GameState.h"
#include "colorcodes.h"
#include "InputManager.h"
#include "Player.h" 
#include "Store.h"

using namespace std;

SaveRoom::SaveRoom(string narrative, Store* roomStore) 
    : Room(narrative, "SaveRoom"), store(roomStore), roomWidth(15), roomHeight(7) {
    
    this->isCleared = true; 
    
    // merchant would be at the top center
    merchantPos = {roomWidth / 2, 1};     
    // campfire would be (almost) exactly at center    
    campfirePos = {roomWidth / 2, 3};       
    // the door woudl be at bottom center
    doorPos = {roomWidth / 2, roomHeight - 1}; 
    // the typewriter is lowkey at top left 
    typewriterPos = {2, 1};                    
}

SaveRoom::~SaveRoom() {
    if (store != nullptr) {
        delete store; 
    }
}

void SaveRoom::render() {

    // pretty UI rendering !! >:D

    cout << "\033[2J\033[1;1H";     // ACSII code that clears screen

    cout << CYAN << "   =======================================\n" << RESET;
    cout << CYAN << "   ||" << RESET << "             SAFE HAVEN              " << CYAN << "||\n" << RESET;
    cout << CYAN << "   =======================================\n" << RESET;

    for (int y = 0; y < roomHeight; ++y) {
        cout << "  "; 
        for (int x = 0; x < roomWidth; ++x) {
            
            if (x == 0 || x == roomWidth - 1 || y == 0 || y == roomHeight - 1) {
                if (x == doorPos.x && y == doorPos.y) {
                    cout << CYAN << "[D]" << RESET;
                } else {
                    cout << GRAY << " # " << RESET;
                }
            } 
            // drawing entities inside the room
            else if (x == localPlayer.x && y == localPlayer.y) {
                cout << GREEN << "[P]" << RESET;
            } else if (x == merchantPos.x && y == merchantPos.y) {
                cout << YELLOW << "[M]" << RESET;
            } else if (x == campfirePos.x && y == campfirePos.y) {
                cout << RED << " * " << RESET; // Campfire
            } else if (x == typewriterPos.x && y == typewriterPos.y) {
                cout << MAGENTA << "[T]" << RESET; // Typewriter
            } else {
                cout << " . "; // Floor
            }
        }
        cout << "\n";
    }
    cout << "\n Legend: " << GREEN << "[P]" << RESET << " You | " 
         << YELLOW << "[M]" << RESET << " Merchant | " 
         << RED << "*" << RESET << " Rest | " 
         << MAGENTA << "[T]" << RESET << " Save | " 
         << CYAN << "[D]" << RESET << " Door\n";
}

void SaveRoom::interact(Player* player) {
    if (!player) return;

    localPlayer = {doorPos.x, doorPos.y - 1}; 
    bool inRoom = true;
    string actionMessage = narrativeText; 

    while (inRoom) {
        render();
        
        if (actionMessage != "") {
            cout << "\n > " << actionMessage << "\n";
            actionMessage = ""; 
        }

        static bool firstSave = true;      // this will change once and will remain changed for every upcoming call
        if(firstSave) {
            cout << "\n" << BLUE << "You: " << RESET << BOLD << " \"Woah what is this room? It looks cozy- There's a campfire, a typewriter... and a merchant? Who looks rather familiar?!\"\n" << RESET;
            InputManager::pause();
            firstSave = false;
            render();
        }

        cout << "\n Action (W/A/S/D to move, Q to quit): ";

        // this uses the keystroke header i put
        char input = InputManager::getChar();

        int dx = 0, dy = 0;

        // just movement mechanics here, WASD translates to these chanes in coordinates
        switch (input) {
            case 'w': case 'W': dy = -1; break;
            case 's': case 'S': dy = 1; break;
            case 'a': case 'A': dx = -1; break;
            case 'd': case 'D': dx = 1; break;
            case 'q': case 'Q': inRoom = false; continue; // Quick escape
            default: 
                actionMessage = "Invalid command. Use WASD.";
                continue;
        }

        Coordinate nextPos = {localPlayer.x + dx, localPlayer.y + dy};

        // if we are on top of the merchant then store opens
        if (nextPos.x == merchantPos.x && nextPos.y == merchantPos.y) {
            if (store != nullptr) {
                store->openInterface(player); 
                actionMessage = "You finished trading with the Merchant.";
            } else {
                actionMessage = "The merchant's stall is strangely empty.";
            }
            continue; 
        }

        //  if we are on top of the campfire, our mana is restored
        if (nextPos.x == campfirePos.x && nextPos.y == campfirePos.y) {
            *player + player->getMaxHP(); 
            player->restoreMana(player->getMaxMana()); 
            
            actionMessage = "You rested by the fire. HP and Mana fully restored.";
            continue; 
        }

        // if we are on top of the typewriter, it gives us the option to save
        if (nextPos.x == typewriterPos.x && nextPos.y == typewriterPos.y) {
            cout << "\033[2J\033[1;1H"; // ANSI Clear Screen
            cout << MAGENTA << "\n--- Typewriter ---\n" << RESET;
            cout << "An old mechanical typewriter rests on a dusty desk.\n";
            cout << "Enter save slot (1-3) or 0 to cancel: ";
            
            int slot;
            cin >> slot;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                // this is kinda funny
                actionMessage = "You fumbled with the keys and gave up.";
                continue;
            }
            cin.ignore(10000, '\n');
            
            if (slot > 0 && slot <= 3) {
                GameState::getInstance()->save(slot);
                // "click clack ding" holy- ;-;
                actionMessage = " *Click Clack Ding* Game successfully saved to slot " + to_string(slot) + ".";
            } else {
                actionMessage = "You stepped away from the typewriter.";
            }
            continue; 
        }

        // if we are on door then we exit
        if (nextPos.x == doorPos.x && nextPos.y == doorPos.y) {
            inRoom = false; 
            continue;
        }

        // bounds check
        if (nextPos.x > 0 && nextPos.x < roomWidth - 1 && 
            nextPos.y > 0 && nextPos.y < roomHeight - 1) {
            localPlayer = nextPos;
        } else {
            actionMessage = "You bumped into the wall.";
        }
    }
}