#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include "Item.h"
#include "Player.h"

class Consumable : public Item
{
public:
    Consumable(const std::string &name = "Unknown Item", int value = 0);
    Consumable(const Consumable &other);
    virtual ~Consumable() = default;

    virtual void use(Player *target) override = 0; // pure virtual

    void serialize(json& j) const override {
        Item::serialize(j);
    }

    void deserialize(const json& j) override {
        Item::deserialize(j);
    }
};

class HealthPotion : public Consumable
{
private:
    int healAmount;

public:
    HealthPotion(int healAmount = 25, int value = 30);
    HealthPotion(const HealthPotion &other);

    void use(Player *target) override;
    std::string getType() const override;
    int getHealAmount() const;

    void serialize(nlohmann::json& j) const override {
        Consumable::serialize(j);       
        j["type"] = "HealthPotion";    
        j["healAmount"] = healAmount;
    }

    void deserialize(const nlohmann::json& j) override {
        Consumable::deserialize(j);
        healAmount = j.value("healAmount", 25);
    }
};

class DamagePotion : public Consumable {
    private:
        int damageBuffAmount;

    public:
        DamagePotion(int damageBuffAmount = 10, int value = 40);
        DamagePotion(const DamagePotion &other);
        void use(Player *target) override;
        std::string getType() const override;
        int getDamageBuffAmount() const;
        void serialize(json& j) const override {
            Consumable::serialize(j);
            j["type"] = "DamagePotion"; 
            j["damageBuffAmount"] = damageBuffAmount;
        }

        void deserialize(const json& j) override {
            Consumable::deserialize(j);
            damageBuffAmount = j.value("damageBuffAmount", 10);
        }
};


class ManaPotion : public Consumable {
    private:
        int restoreAmount;

    public:
        ManaPotion(int restoreAmount =25, int value = 25);
        ManaPotion(const ManaPotion &other);
        void use(Player *target) override;
        std::string getType() const override;
        int getRestoreAmount() const;
        void serialize(json& j) const override {
            Consumable::serialize(j);
            j["type"] = "ManaPotion"; 
            j["restoreAmount"] = restoreAmount;
        }

        void deserialize(const json& j) override {
            Consumable::deserialize(j);
            restoreAmount = j.value("restoreAmount", 25);
        }
};

class DefensePotion : public Consumable {
    private:
        int defenseBuffAmount;

    public:
        DefensePotion(int defenseBuffAmount, int value = 35);
        DefensePotion(const DefensePotion &other);
        void use(Player *target) override;
        std::string getType() const override;
        int getDefenseBuffAmount() const;
        void serialize(json& j) const override {
            Consumable::serialize(j);       
            j["type"] = "DefensePotion";    
            j["defenseBuffAmount"] = defenseBuffAmount;
        }

        void deserialize(const json& j) override {
            Consumable::deserialize(j);
            defenseBuffAmount = j.value("defenseBuffAmount", 25);
        }
};

#endif