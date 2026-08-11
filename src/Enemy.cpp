#include "Enemy.h"
#include <iostream>
#include "LootTable.h"
#include "StatusEffect.h"
#include "StatModifier.h"

Enemy::Enemy() {
    lootTable = nullptr;
    activeEffects = nullptr;
    numEffects = 0;
    effectCapacity = 3;

    lootTable = nullptr;
    activeEffects = nullptr;
    numEffects = 0;
    effectCapacity = 3;
    
    // Initialize Modifiers
    activeModifiers = nullptr;
    numModifiers = 0;
    modifierCapacity = 5;
}

Enemy::~Enemy() {
    if (activeEffects != nullptr) {
        delete[] activeEffects;
        activeEffects = nullptr; 
    }
    
    if (activeModifiers != nullptr) {
        delete[] activeModifiers;
        activeModifiers = nullptr;
    }
    
    if (lootTable != nullptr) {
        delete lootTable;
        lootTable = nullptr;
    }
}


// Provided logic for completeness, simulated return syntax assuming full definition elsewhere
// (Code compilation may fail here specifically if compiled without struct definitions included)
LootBundle Enemy::dropLoot() {
    if (lootTable != nullptr) {
        return lootTable->roll();
    }
    return LootBundle(10, 5, 2); 
}


void Enemy::addStatusEffect(StatusEffect effect) {
    if (numEffects < effectCapacity) {
        
        if (activeEffects == nullptr) {
            activeEffects = new StatusEffect[effectCapacity];
        }
        
        activeEffects[numEffects] = effect;
        numEffects++;
    } else {
        std::cout << name << " is already suffering from maximum status effects!\n";
    }
}

void Enemy::processStatusEffects() {
    for (int i = 0; i < numEffects; i++) {
        activeEffects[i].tick(this);
    }
    
    int newCount = 0;
    for (int i = 0; i < numEffects; i++) {
        if (activeEffects[i].duration > 0) { 
            activeEffects[newCount++] = activeEffects[i];
        } else {
            std::cout << name << " recovered from a status condition!\n";
        }
    }
    numEffects = newCount;
}


void Enemy::applyModifier(StatModifier mod) {
    if (numModifiers < modifierCapacity) {
        if (activeModifiers == nullptr) {
            activeModifiers = new StatModifier[modifierCapacity];
        }
        activeModifiers[numModifiers] = mod; 
        numModifiers++;
        std::cout << name << " gained a stat boost!\n";
    }
}

void Enemy::takeDamage(int amount) {
    int actualDamage = amount;

    if (actualDamage > 0) {
        for (int i = 0; i < numModifiers; i++) {
            if (activeModifiers[i].getStatType() == DEFENSE) {
                actualDamage -= activeModifiers[i].getValue();
            }
        }
    }

    if (actualDamage < 1 && amount > 0) actualDamage = 1; 

    Entity::takeDamage(actualDamage); 
}




void Enemy::serialize(json& j) const {
    Entity::serialize(j);
    j["name"] = name;
}
void Enemy::deserialize(const json& j) {
    Entity::deserialize(j);
    name = j.value("name", "Unknown");

    numEffects = 0;
    if (activeEffects != nullptr) {
        delete[] activeEffects; 
        activeEffects = nullptr; 
    }
    effectCapacity = 5; 
    activeEffects = new StatusEffect[effectCapacity];
}

