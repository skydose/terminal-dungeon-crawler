#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>
#include "Serializable.h"
#include "Player.h"

class Item : public Serializable
{
protected:
    std::string name;
    int value;

public:
    static int totalItemsCreated;

    Item(const std::string &name = "Unknown Item", int value = 0);
    Item(const Item &other); // copy constructor
    virtual ~Item();

    virtual std::string getType() const = 0; // pure virtual

    std::string getName() const;
    int getValue() const;
    void setValue(int v);

    virtual void use(Player *target) = 0; // pure virtual

    // Comparison by value
    bool operator<(const Item &other) const;
    bool operator>(const Item &other) const;
    // Stream output
    friend std::ostream &operator<<(std::ostream &os, const Item &item);

    virtual void serialize(nlohmann::json &j) const override {
        j["name"] = name;
        j["value"] = value;
    }

    virtual void deserialize(const nlohmann::json& j) override {
        name = j.value("name", "Unknown Item");
        value = j.value("value", 0);
    }

};

#endif