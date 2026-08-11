#ifndef SPELL_H
#define SPELL_H

#include <string>
#include "Serializable.h" 
#include "StatusEffect.h"
class Entity;

class Spell : public Serializable {
protected:
    std::string name;
    int manaCost;

public:
    static float globalManaTax;

    Spell(std::string n = "Unknown Spell", int m = 0);
    virtual ~Spell() = default;

    std::string getName() const;
    int getManaCost() const;

    //both are now pure virtual
    virtual void cast(Entity* caster, Entity* target) = 0;
    virtual void effect(Entity* caster, Entity* target) = 0;
    virtual void serialize(json& j) const override {
        j["name"] = name;
        j["manaCost"] = manaCost;
    }

    virtual void deserialize(const json& j) override {
        name = j.value("name", "Unknown Spell");
        manaCost = j.value("manaCost", 0);
    }
};

class HealingSpell : public Spell {
private:
    int healAmount;

public:
    HealingSpell(std::string n = "Heal", int m = 10, int h = 25);
    void cast(Entity* caster, Entity* target) override;
    void effect(Entity*, Entity* target) override;

    void serialize(json& j) const override {
        Spell::serialize(j);
        j["type"] = "HealingSpell"; 
        j["healAmount"] = healAmount;
    }

    void deserialize(const json& j) override {
        Spell::deserialize(j);
        healAmount = j.value("healAmount", 25);
    }
};

class DamageSpell : public Spell {
private:
    int damage;
    bool causesStatus;
    StatusType statusType;
    int statusDuration;
    int statusTickValue;

public:
    DamageSpell(std::string n = "Fireball", int m = 15, int d = 30, bool hasStatus = false, StatusType sType = BURN, int sDur = 0, int sTick = 0);
    void cast(Entity* caster, Entity* target) override;
    void effect(Entity* , Entity* target) override;

    void serialize(json& j) const {
        Spell::serialize(j);
        j["type"] = "DamageSpell"; 
        j["damage"] = damage;
        j["causesStatus"] = causesStatus;
        
        if (causesStatus) {
            j["statusType"] = statusType;
            j["statusDuration"] = statusDuration;
            j["statusTickValue"] = statusTickValue;
        }
    }

    void deserialize(const json& j) {
        Spell::deserialize(j);
        damage = j.value("damage", 30);
        causesStatus = j.value("causesStatus", false);
        
        if (causesStatus) {
            statusType = j.value("statusType", BURN);
            statusDuration = j.value("statusDuration", 3);
            statusTickValue = j.value("statusTickValue", 1);
        }
    }
};


#endif