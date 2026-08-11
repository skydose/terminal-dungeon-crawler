#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Player.h"
#include "MapGrid.h" 
#include "json.hpp"
#include <string>


//  this is a singleton so i could manage the game state like some sorta database
// im not going to explain all the singleton structure because we already studied that
class GameState {
private:
    static GameState* instance;
    
    Player* player;
    MapGrid* map;


    GameState();
public:
    // delete copy constructors for strict singleton enforcement
    GameState(const GameState&) = delete;
    void operator=(const GameState&) = delete;

    ~GameState();

    static GameState* getInstance();

    void setPlayer(Player* p);
    void setMap(MapGrid* m);

    Player* getPlayer();
    MapGrid* getMap(); 

    void save(int slot);
    bool load(int slot);

    void cleanup();
};

#endif