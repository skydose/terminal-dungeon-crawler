#include "Entity.h"
#include <iostream>
#include <cstdlib>
#include "StatModifier.h"

int Entity::totalEntities = 0;

Entity::Entity() : name("Unknown Entity"), maxHP(100), currentHP(100), level(1), critRate(0.1f), critMultiplier(1.5f) {
    totalEntities++;
    hpBar.update(currentHP, maxHP);
}

Entity::~Entity() {
    totalEntities--;
}

std::string Entity::getName() const { return name; }
int Entity::getHP() const { return currentHP; }
int Entity::getMaxHP() const { return maxHP; }
int Entity::getLevel() const { return level; }

void Entity::takeDamage(int amount) {
    currentHP -= amount;
    if (currentHP < 0) currentHP = 0;
    if (currentHP > maxHP) currentHP = maxHP;
    
    // Feed internal Composition object data
    hpBar.update(currentHP, maxHP);
    
    std::cout << name << " drops to " << currentHP << "/" << maxHP << " HP.\n";
}

bool Entity::isDead() const {
    return currentHP <= 0;
}

int Entity::calculateDamage(int base) const {
    int finalDamage = (base < 0) ? 0 : base; // Clamp negative scaling bounds reliably
    float randVal = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    
    if (randVal <= critRate) {
        finalDamage = static_cast<int>(base * critMultiplier);
        std::cout << ">>> " << name << " CRITICAL STRIKES (" << finalDamage << " dmg)! <<<\n";
    }
    return finalDamage;
}

// Global operator<< overload processing custom output syntax natively
std::ostream& operator<<(std::ostream& os, const Entity& e) {
    os << "[" << e.name << " Lvl:" << e.level << " " << e.hpBar.getVisual(10, "\033[32m") << "\033[0m]";
    return os;
}


void Entity::serialize(json& j) const {
    j["name"] = name;
    j["maxHP"] = maxHP;
    j["currentHP"] = currentHP;
    j["level"] = level;
    j["critRate"] = critRate;
    j["critMultiplier"] = critMultiplier;
}
void Entity::deserialize(const json& j) {
    name = j.value("name", "Unknown");
        
    setMaxHP(j.value("maxHP", 100));            // essential for the health bar
    setCurrentHP(j.value("currentHP", maxHP)); 
    
    level = j.value("level", 1);
    critRate = j.value("critRate", 0.05f);
    critMultiplier = j.value("critMultiplier", 1.5f);
}