#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H
#include "MapGrid.h"
#include "Store.h"

//  a singleton (the second singleton in my branch :'( )
class WorldGenerator {
    static WorldGenerator* instance;
    WorldGenerator();
    WorldGenerator(const WorldGenerator& other) = delete;
    void operator=(const WorldGenerator&) = delete;
    Enemy* spawnRandomEnemy();
public:
    static WorldGenerator* getInstance();
    MapGrid* generateMap(int w, int h);
    void placeRooms(MapGrid* map);

    Store* createStockedStore();
};

#endif