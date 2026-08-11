#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <iostream>
#include "Serializable.h"


class HealthBar {
private:
    int current;
    int max;
public:
    HealthBar() : current(0), max(1) {}
    
    void update(int currentHP, int maxHP) {
        current = currentHP;
        max = maxHP;
    }
    
    std::string getVisual(int length, std::string colorCode) const {
    if (max <= 0) return "[ ERROR ]";
    
    float ratio = static_cast<float>(current) / max;
    if (ratio < 0) ratio = 0;
    if (ratio > 1) ratio = 1;

    int filledBlocks = ratio * length;
    int emptyBlocks = length - filledBlocks;

    std::string bar = colorCode + "[";
    for (int i = 0; i < filledBlocks; i++) bar += "█"; 
    
    bar += "\033[0m"; // ANSI Reset to default
    for (int i = 0; i < emptyBlocks; i++) bar += "\033[90m░"; // ANSI code gray for empty space
    
    bar += colorCode + "]\033[0m"; // close bracket in original color, then reset

    return bar;
}
};

class Entity: public Serializable {
protected:
    std::string name;
    int maxHP;
    int currentHP;
    int level;
    float critRate;
    float critMultiplier;

    // Direct value-type instance representing Strict OOP Composition
    HealthBar hpBar;

public:
    static int totalEntities;

    Entity();
    virtual ~Entity();

    // Standard Accessors (Getters & Setters)
    std::string getName() const;
    void setName(const std::string& n) { name = n; }

    int getHP() const;
    void setCurrentHP(int hp) { currentHP = hp; hpBar.update(currentHP, maxHP); }

    int getMaxHP() const;
    void setMaxHP(int hp) { maxHP = hp; hpBar.update(currentHP, maxHP); }

    int getLevel() const;
    void setLevel(int lvl) { level = lvl; }

    float getCritRate() const { return critRate; }
    void setCritRate(float rate) { critRate = rate; }

    float getCritMultiplier() const { return critMultiplier; }
    void setCritMultiplier(float mult) { critMultiplier = mult; }

    void takeDamage(int amount);
    bool isDead() const;
    int calculateDamage(int base) const;
    
    // Abstract Runtime Polymorphic signature
    virtual void takeTurn(Entity* target = nullptr) = 0;

    // Sub-goal: Operator overloading directly mapped via friend stream insertion
    friend std::ostream& operator<<(std::ostream& os, const Entity& e);

    virtual void serialize(json& j) const override;
    virtual void deserialize(const json& j) override;

    std::string getHealthBar(int length, std::string colorCode) const {
        return hpBar.getVisual(length, colorCode);
    }
};

#endif // ENTITY_H
