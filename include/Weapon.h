#ifndef WEAPON_H
#define WEAPON_H

#include "Equippable.h"
class Player;



class Weapon : public Equippable
{
protected:
    int baseDamage;

public:
    Weapon(const std::string &name = "Iron Sword", int baseDamage = 10, int value = 5, int maxDurability = 100);
    Weapon(const Weapon &other);

    void onEquip(Player *target) override;
    void onUnequip(Player *target) override;
    void onHit() override;

    std::string getType() const override { return "Weapon"; }
    int getBaseDamage() const;
    void setBaseDamage(int d);

    void serialize(json& j) const override {
        Equippable::serialize(j);
        j["type"] = "Weapon"; 
        j["baseDamage"] = baseDamage;
    }

    void deserialize(const json& j) override {
        Equippable::deserialize(j);
        baseDamage = j.value("baseDamage", 10);
    }
};


class Dagger : public Weapon
{
private:
    int poisonChance; // 0-100

public:
    Dagger(const std::string &name = "Dagger", int baseDamage = 6, int poisonChance = 15, int value = 90);
    Dagger(const Dagger &other);
    void onHit() override;
    std::string getType() const override;
    int getPoisonChance() const;
    void serialize(json& j) const override {
        Weapon::serialize(j);
        j["type"] = "Dagger";
        j["poisonChance"] = poisonChance;
    }

    void deserialize(const json& j) override {
        Weapon::deserialize(j);
        poisonChance = j.value("poisonChance", 15);
    }
};

class Sword : public Weapon
{
private:
    float lifeStealRatio;

public:
    Sword(const std::string &name = "Iron Sword", int baseDamage = 10, float lifeStealRatio = 0.1f, int value = 120);
    Sword(const Sword &other);
    void onEquip(Player *target) override;
    std::string getType() const override;
    float getLifeStealRatio() const;
    void serialize(nlohmann::json& j) const override {
        Weapon::serialize(j);       
        j["type"] = "Sword";           
        j["lifeStealRatio"] = lifeStealRatio;
    }

    void deserialize(const nlohmann::json& j) override {
        Weapon::deserialize(j);
        lifeStealRatio = j.value("lifeStealRatio", 0.1f);
    }
};

class Axe : public Weapon
{
private:
    int cleaveBonus;

public:
    Axe(const std::string &name = "Iron Axe", int baseDamage = 12, int cleaveBonus = 5, int value = 150);
    Axe(const Axe &other);
    void onHit() override;
    std::string getType() const override;
    int getCleaveBonus() const;
    void serialize(json& j) const override {
        Weapon::serialize(j);
        j["type"] = "Axe"; 
        j["cleaveBonus"] = cleaveBonus;
    }

    void deserialize(const json& j) override {
        Weapon::deserialize(j);
        cleaveBonus = j.value("cleaveBonus", 5);
    }
};


#endif