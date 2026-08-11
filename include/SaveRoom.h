#ifndef SAVEROOM_H
#define SAVEROOM_H
#include "Room.h"
#include "Coordinate.h"
#include <string>

class Player;
class Store;

class SaveRoom : public Room {
    Store* store;
    int roomWidth;
    int roomHeight;
    Coordinate localPlayer;
    Coordinate merchantPos;
    Coordinate doorPos;
    Coordinate campfirePos;
    Coordinate typewriterPos;

    void render();
    bool isAdjacent(Coordinate a, Coordinate b) {
        int dx = abs(a.x - b.x);
        int dy = abs(a.y - b.y);
        return (dx <= 1 && dy <= 1);
    }
public:

    // constructor
    SaveRoom(string text, Store* store);
    ~SaveRoom() override;
    void interact(Player* player) override;
};

#endif