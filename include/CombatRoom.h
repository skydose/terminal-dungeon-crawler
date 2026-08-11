#ifndef COMBATROOM_H
#define COMBATROOM_H

#include "Room.h"

#include "Serializable.h"

#include "Coordinate.h"

class Enemy;
class Player;

class CombatRoom : public Room {
private:
    Enemy** enemies;        // array of enemies
    int numEnemies;         // number of enemies
    int currentEnemyIndex;  // since we are fighting them one by one

    Item** floorLoot;
    int floorCapacity;
    int numFloorItems;

    int roomWidth;
    int roomHeight;
    Coordinate localPlayer;
    Coordinate doorPos;

    void render();

    void collectFloorLoot(Player* player); // should only be used inside the class
public:
    CombatRoom(string text, Enemy** enemyGroup, int groupSize);
    ~CombatRoom() override;

    void enter(Player* player) override;
    void interact(Player* player) override;
    void serialize(json &j) const override;
    void deserialize(const json& j) override;
};

#endif