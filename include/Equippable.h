#ifndef EQUIPPABLE_H
#define EQUIPPABLE_H

#include "Item.h"
#include "Player.h"

class Equippable : public Item
{
protected:
    int durability;
    int maxDurability;

public:
    Equippable(const std::string &name = "Unknown Gear", int value = 0, int maxDurability = 0);
    Equippable(const Equippable &other);
    virtual ~Equippable() = default;

    virtual void onEquip(Player *target) = 0;
    virtual void onUnequip(Player *target) = 0;
    virtual void onHit() = 0;
    int getDurability() const;
    int getMaxDurability() const;
    bool isBroken() const;
    void repair(int amount);
    void degrade(int amount);

    void use(Player*) override { 
        std::cout << ">> You cannot 'use' this item. It must be equipped!\n";
    }

    void serialize(nlohmann::json& j) const override {
        Item::serialize(j);
        j["durability"] = durability;
        j["maxDurability"] = maxDurability;
    }

    void deserialize(const nlohmann::json &j) override {
        Item::deserialize(j);
        durability = j.value("durability", 0);
        maxDurability = j.value("maxDurability", 0);
    }


};



#endif