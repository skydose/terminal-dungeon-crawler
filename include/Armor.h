#ifndef ARMOR_H
#define ARMOR_H

#include "Equippable.h"


class Armor : public Equippable
{
protected:
    int baseDefense;

public:
    Armor(const std::string &n = "Leather Armor", int def = 5, int val = 5, int maxDur = 100);
    Armor(const Armor &other);

    void onEquip(Player *target) override;
    void onUnequip(Player *target) override;
    void onHit() override;

    std::string getType() const override;
    int getBaseDefense() const;
    void setBaseDefense(int d);

    void serialize(json& j) const override {
        Equippable::serialize(j);
        j["type"] = "Armor"; 
        j["baseDefense"] = baseDefense;
    }

    void deserialize(const json& j) override {
        Equippable::deserialize(j);
        baseDefense = j.value("baseDefense", 5);
    }
};

class PlateArmor : public Armor
{
private:
    float weightPenalty;

public:
    PlateArmor(const std::string &name = "Iron Plate", int baseDefense = 15, float weightPenalty = 0.2f, int value = 250);
    PlateArmor(const PlateArmor &other);
    void onEquip(Player *target) override;
    std::string getType() const override;
    float getWeightPenalty() const;
    void serialize(nlohmann::json& j) const override {
        Armor::serialize(j);          
        j["type"] = "PlateArmor";     
        j["weightPenalty"] = weightPenalty;
    }

    void deserialize(const nlohmann::json& j) override {
        Armor::deserialize(j);
        weightPenalty = j.value("weightPenalty", 0.2f);
    }
};

class ChainMail : public Armor
{
private:
    int linkedRings;

public:
    ChainMail(const std::string &name = "Chainmail", int baseDefense = 10, int linkedRings = 1000, int value = 130);
    ChainMail(const ChainMail &other);
    void onEquip(Player *target) override;
    std::string getType() const override;
    int getLinkedRings() const;
    void serialize(json& j) const override {
        Armor::serialize(j);
        j["type"] = "ChainMail"; 
        j["linkedRings"] = linkedRings;
    }

    void deserialize(const json& j) override {
        Armor::deserialize(j);
        linkedRings = j.value("linkedRings", 1000);
    }
};

#endif