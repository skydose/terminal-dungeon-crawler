#ifndef EMPTYROOM_H
#define EMPTYROOM_H
#include "Room.h"
#include "Player.h"
#include "Coordinate.h"
#include <string>
#include <iostream>
using namespace std;

class EmptyRoom : public Room {
private:
    int roomWidth;
    int roomHeight;
    Coordinate localPlayer;
    Coordinate doorPos;

    void render();

public:
    EmptyRoom(std::string narrative);
    ~EmptyRoom() override = default;

    void interact(Player* player) override;
};

#endif