#include "WorldGenerator.h"
#include "CombatRoom.h"
#include "EmptyRoom.h"
#include "SaveRoom.h"
#include "WorldRules.h"
#include "Boss.h"
#include "Enemy.h"
#include "Goblin.h"
#include "Armor.h"
#include "Weapon.h"

#include "Knight.h"
#include <cstdlib>
#include <iostream>
#include "Consumable.h"

using namespace std;

WorldGenerator* WorldGenerator::instance = nullptr;

WorldGenerator::WorldGenerator() {}

WorldGenerator* WorldGenerator::getInstance() {
    if(instance == nullptr) instance = new WorldGenerator();
    return instance;
}

MapGrid* WorldGenerator::generateMap(int w, int h) {
    cout << "Generating a map with seed " << WorldRules::worldSeed << "...\n";
    srand(WorldRules::worldSeed);       // setting the seed

    MapGrid* map = new MapGrid(w, h);
    placeRooms(map);

    return map;
}

//  basically an enemy randomizer for the world gen (wish we had more variety)
Enemy* WorldGenerator::spawnRandomEnemy() {
    int rng = rand() % 100;
    
    if(rng < 70) return new Goblin();
    else return new Knight(); 
}
void WorldGenerator::placeRooms(MapGrid* map) {
    
    for(int x = 0; x < WorldRules::mapWidth; x++) {
        for(int y = 0; y < WorldRules::mapHeight; y++ ) {
            
            // since i want the beginning rooms to be tutorial rooms so the first few rooms are pre-determined the rest are randomized like:
            // (0,0) would be an empty room so the player knows how to move around
            if(x == 0 && y == 0) {
                map->setRoom(x, y, new EmptyRoom("You enter the dungeon, it's big and cold- definitely not friendly either."));
                continue;
            }

            // then the room in the bottom right section would be a boss room (where we need to go)
            if (x == WorldRules::mapWidth - 1 && y == WorldRules::mapHeight - 1) {
                Enemy** bossGroup = new Enemy*[1];
                bossGroup[0] = new Boss();
                map->setRoom(x, y, new CombatRoom("A chilling aura fills the air... The Lich Overlord awaits!", bossGroup, 1));
                continue;
            }

            // then below the (0,0) and to the right of it is a simple and easy combat room with just one Goblin
            if((x == 1 && y == 0) || (x == 0 && y == 1)) {
                Enemy** enemies = new Enemy*[1];
                enemies[0] = new Goblin();
                map->setRoom(x, y, new CombatRoom("As expected- there are enemies lurking here!", enemies, 1));
                continue;
            }

            // then between them would be a save room (hopefully the player is more prompted to go to it)
            if(x == 1 && y == 1) {
                Store* store = createStockedStore(); 
                map->setRoom(x, y, new SaveRoom("An unusual room? A merchant sits quietly.", store));
                continue;
            }

            // then the rest would be randomized rooms, like rng
            int rng = rand() % 100;
            
            if(rng < 60) {
                // 60% Chance: Combat Room
                int numEnemies = (rand() % 3) + 1;      
                Enemy** enemies = new Enemy*[numEnemies];

                for(int i = 0; i < numEnemies; i++) {
                    enemies[i] = spawnRandomEnemy(); //  50/50 Goblins & Knights
                }
                map->setRoom(x, y, new CombatRoom("Enemies block your path...", enemies, numEnemies));
            }
            else if(rng < 70) {
                Store* store = createStockedStore();
                map->setRoom(x, y, new SaveRoom("A safe haven. You can catch your breath here.", store));
            }
            else if(rng < 85) {
                map->setRoom(x, y, new EmptyRoom("A quiet, empty section of the dungeon."));
            }
            else {
            }
        }
    }
}


// assets for the store
Store* WorldGenerator::createStockedStore() {
    Store* store = new Store(15); 

    store->stockItem(new Sword("Iron Sword", 45, 20));
    store->stockItem(new Axe("Stone Axe", 35, 25));
    store->stockItem(new Dagger("Theif's Dagger", 15, 12));

    store->stockItem(new ChainMail());
    store->stockItem(new PlateArmor());

    store->stockItem(new HealthPotion(50, 20));
    store->stockItem(new HealthPotion(100, 45));
    store->stockItem(new ManaPotion(50, 20));
    store->stockItem(new DamagePotion(25, 30));
    store->stockItem(new DefensePotion(15, 25));

    return store;
}