#include "GameState.h"
#include "colorcodes.h" 
#include <iostream>
#include <fstream>      
#include <filesystem>

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

GameState* GameState::instance = nullptr;

GameState::GameState() : player(nullptr), map(nullptr) {
   
}

GameState::~GameState() {
    cleanup(); // deletes the player and the map as it closes
}

GameState* GameState::getInstance() {
    if(instance == nullptr) instance = new GameState();
    return instance;
}

void GameState::setPlayer(Player* p) { player = p; }
void GameState::setMap(MapGrid* m) { map = m; }
Player* GameState::getPlayer() { return player; }
MapGrid* GameState::getMap() { return map; }



void GameState::save(int slot) {
    json j;
    

    // delegating player and map, then serializing them individually
    if(player != nullptr) {
        json playerJson;
        player->serialize(playerJson);
        j["player"] = playerJson;
    }

    if(map != nullptr) {
        json mapJson;
        map->serialize(mapJson);
        j["map"] = mapJson;
    }

    if(!fs::exists("saves")) {
        fs::create_directory("saves");
    }

    // deciding the slot to save in (player usually gives 1 to 3)
    string filename = "saves/save" + to_string(slot) + ".json";
    ofstream outFile(filename);
    
    if(outFile.is_open()) {
        outFile << j.dump(4); 
        outFile.close();
    } else {
        cout << RED << "Error: Could not save to " << filename << RESET << endl;
    }
}

bool GameState::load(int slot) {
    // deciding which slot we access
    string filename = "saves/save" + to_string(slot) + ".json";
    ifstream inFile(filename);
    
    if(!inFile.is_open()) {
        return false; // letting the menu know this save file is empty
    }

    json j;
    try {
        inFile >> j;

        // if theres parsing error we just return with a false because the file is prob corrupted
    } catch (json::parse_error& e) {
        cout << RED << "Save file is corrupted!\n" << RESET << endl;
        return false;
    }
    inFile.close();



    // first checks if the json file contains a player/map, it deserializes it otherwise skips
    if(j.contains("player") && player != nullptr) {
        player->deserialize(j["player"]);
    }

    if(j.contains("map") && map != nullptr) {
        map->deserialize(j["map"]);
    }

    return true; 
}


// doing the destructor's work
void GameState::cleanup() {
    if(player != nullptr) {
        delete player;
        player = nullptr;
    }
    if(map != nullptr) {
        delete map;
        map = nullptr;
    }
}