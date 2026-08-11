#ifndef ROOM_H
#define ROOM_H
#include "Enemy.h"
#include "Store.h"
#include <string>
#include "Serializable.h"


using namespace std;

class Room : public Serializable{
protected:
    string narrativeText;
    string roomType;
    bool isCleared;
    
public:
    Room(string text, string roomType);
    virtual ~Room() {}      // has a virtual destructor for the derived classes
    virtual void enter(Player*);
    virtual void interact(Player* player) = 0;
    virtual bool isSaveRoom() const;      // not every room is a saveroom
    void serialize(json& j) const override {
        j["type"] = roomType;
        j["narrative"] = narrativeText;
        j["isCleared"] = isCleared;
    }

    void deserialize(const json& j) override {
        isCleared = j.value("isCleared", false); 
    }

    bool getIsCleared() { return isCleared; }
};

#endif