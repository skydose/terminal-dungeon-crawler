#include "EmptyRoom.h"
#include "colorcodes.h"
#include <iostream>
#include <limits>
#include "InputManager.h"

using namespace std;

EmptyRoom::EmptyRoom(string narrative) : Room(narrative, "EmptyRoom") {
    isCleared = true; // Always cleared since it's empty
    roomWidth = 15;
    roomHeight = 7;
    doorPos = {roomWidth / 2, roomHeight - 1}; // Door at the bottom center
}

void EmptyRoom::render() {
    cout << "\033[2J\033[1;1H"; // ANSI clear screen

    // Eerie gray borders for the empty room
    cout << GRAY << "   =======================================\n" << RESET;
    cout << GRAY << "   ||" << RESET << "            DUSTY CORRIDOR           " << GRAY << "||\n" << RESET;
    cout << GRAY << "   =======================================\n" << RESET;

    for (int y = 0; y < roomHeight; ++y) {
        cout << "  "; 
        for (int x = 0; x < roomWidth; ++x) {
            
            // drawing the walls the door
            if (x == 0 || x == roomWidth - 1 || y == 0 || y == roomHeight - 1) {
                if (x == doorPos.x && y == doorPos.y) {
                    cout << CYAN << "[D]" << RESET;
                } else {
                    cout << GRAY << " # " << RESET;
                }
            } 
            // the player:
            else if (x == localPlayer.x && y == localPlayer.y) {
                cout << GREEN << "[P]" << RESET;
            } 
            // filling the floor
            else {
                cout << GRAY << " . " << RESET;
            }
        }
        cout << "\n";
    }
    cout << "\n Legend: " << GREEN << "[P]" << RESET << " You | " 
         << CYAN << "[D]" << RESET << " Door\n";
}

void EmptyRoom::interact(Player* player) {
    if (!player) return;

    localPlayer = {doorPos.x, doorPos.y - 1}; // spawn right inside the door
    bool inRoom = true;
    string actionMessage = narrativeText; // display the room's text on entry

    while (inRoom) {
        render();
        
        if (actionMessage != "") {
            cout << "\n > " << actionMessage << "\n";
            actionMessage = ""; 
        }

        static bool firstEmpty = true;      // this will change once and will remain changed for every upcoming call
        if(firstEmpty) {
            cout << "\n" << BLUE << "You: " << RESET << BOLD << " \"Uh... This room seems empty, nothing to look for.\"\n" << RESET;
            InputManager::pause();
            firstEmpty = false;
            render();
        }

        cout << "\n Action (W/A/S/D to move, Q to escape): ";
        char input = InputManager::getChar();
        


        int dx = 0, dy = 0;

        switch (input) {
            case 'w': case 'W': dy = -1; break;
            case 's': case 'S': dy = 1; break;
            case 'a': case 'A': dx = -1; break;
            case 'd': case 'D': dx = 1; break;
            case 'q': case 'Q': inRoom = false; continue; // Quick escape
            default: continue; 
        }

        Coordinate nextPos = {localPlayer.x + dx, localPlayer.y + dy};

        if (nextPos.x == doorPos.x && nextPos.y == doorPos.y) {
            inRoom = false; 
            continue;
        }

        if (nextPos.x > 0 && nextPos.x < roomWidth - 1 && 
            nextPos.y > 0 && nextPos.y < roomHeight - 1) {
            localPlayer = nextPos;
        } else {
            actionMessage = "You bumped into the cold stone wall.";
        }
    }
}