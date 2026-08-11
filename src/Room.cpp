#include "Room.h"
#include <iostream>

Room::Room(string text, string roomType) : narrativeText(text), roomType(roomType), isCleared(false) {}

void Room::enter(Player*) {
    cout << ">> You have entered a room.\n";
    cout << narrativeText << endl;
}

bool Room::isSaveRoom() const { return false; }