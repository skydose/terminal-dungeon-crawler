#ifndef MAPGRID_H
#define MAPGRID_H
#include "Serializable.h"
#include "Coordinate.h"
#include "Room.h"
#include "Grid.h"

class Person;

class MapGrid : public Serializable {
    int width;
    int height;
    Grid<Room*> rooms;      // using the template helper class i made to easily manage 2d arrays

    //  we store player coordinates 
    Coordinate playerPosition;
    void clearGrid();
public:
    MapGrid(int w, int h);
    ~MapGrid();
    bool move(int dx, int dy, Player* player);
    void setRoom(int x, int y, Room* room);
    Coordinate getPlayerPosition() const;
    void render();
    void serialize(json& j) const override;   // taking items data currently on RAM and store it in a json file to save the game
    void deserialize(const json& j) override;    // to access the data present in the json files and add them to our program via new
};

#endif