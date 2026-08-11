#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <string>
#include "DynamicArray.h"

class SkillManager;
class Skill;
class Weapon;
class Armor;
class Inventory;
class Spellbook;
class StatModifier;
class Equippable;

class Player : public Entity {
private:
    int xp;
    int gold;
    int mana;
    Inventory* inventory;
    Spellbook* spellbook;
    DynamicArray<StatModifier> activeModifiers;
    int modifierCapacity;
    Weapon* equippedWeapon;
    Armor* equippedArmor;

    DynamicArray<Skill*> unlockedSkills;
    int numUnlockedSkills;
    int skillCapacity;

public:
    Player();
    virtual ~Player() override;

    // Standard Accessors
    int getXP() const { return xp; }
    void setXP(int val) { xp = val; }
    void addXP(int x);

    void healHP(int val) { if(currentHP + val > 100) currentHP = maxHP; else currentHP += val; }


    int getGold() const { return gold; }
    void setGold(int val) { gold = val; }
    void addGold(int val) { gold += val; }
    void deductGold(int cost) { gold -= cost; }

    int getMana() const { return mana; }
    bool setMana(int val) { 
        mana = val; 
        if (mana < 0) mana = 0; 
        if (mana > 100) mana = 100; 
        return true; 
    }
    int getMaxMana() const { return 100; }

    Inventory* getInventory() const { return inventory; }
    void setInventory(Inventory* inv) { inventory = inv; }

    Spellbook* getSpellbook() const { return spellbook; }
    void setSpellbook(Spellbook* sb) { spellbook = sb; }

    Weapon* getEquippedWeapon() const { return equippedWeapon; }
    void setEquippedWeapon(Weapon* w) { equippedWeapon = w; }

    Armor* getEquippedArmor() const { return equippedArmor; }
    void setEquippedArmor(Armor* a) { equippedArmor = a; }
    int getLevel() const { return level; }
    // Core Behaviors
    void attack(Entity* target);
    void useItem(int index);
    void castSpell(int index, Entity* target);
    void equip(Equippable* item);
    void applyModifier(StatModifier mod);
    void clearModifiers();
    void restoreMana(int amount);
    void takeDamage(int amount);
    
    void takeTurn(Entity*) override;

    void addSkill(Skill* skill);
    int getNumUnlockedSkills() const { return numUnlockedSkills; }
    Skill* getUnlockedSkill(int index) const { 
        if (index >= 0 && index < numUnlockedSkills) return unlockedSkills[index]; 
        return nullptr; 
    }

    // OOP Feature: Operator Overloading (+)
    Player& operator+(int healAmount);

    // OOP Feature: Filing Mechanisms
    void saveGameState(const std::string& filename = "save.json");
    void loadGameState(const std::string& filename = "save.json");

    void serialize(json& j) const override;
    void deserialize(const json& j) override;
};

#endif