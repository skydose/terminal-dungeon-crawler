#include "Goblin.h"
#include <iostream>
#include <cstdlib>
#include "Weapon.h"
#include "Consumable.h"
#include "Armor.h"
#include "StatModifier.h"
#include "LootTable.h"

Goblin::Goblin() {
    name = "Goblin";
    maxHP = 40;
    currentHP = 40;
    level = 2;
    critRate = 0.05f;
    critMultiplier = 1.2f;
    stealthChance = 0.35f;

    lootTable = new LootTable(5); 
    lootTable->setXPRange(10, 20);
    lootTable->setGoldRange(5, 12);
    
    lootTable->addEntry(new Dagger("Rusty Dagger", 7, 8), 0.40f); 
    lootTable->addEntry(new HealthPotion(25, 10), 0.25f);
    lootTable->addEntry(new Sword("Wooden Sword", 9, 0.1f, 50), 0.10f);
    lootTable->addEntry(new DefensePotion(15), 0.15f);
    lootTable->addEntry(new Armor(), 0.10f);


}

Goblin::~Goblin() {}

int Goblin::decideAction() {
    float randVal = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    if (randVal <= stealthChance) {
        return 1; // 1 = enter stealth
    }
    return 0; // 0 = physical shank
}

void Goblin::takeTurn(Entity* target) {

    processStatusEffects(); 

    if (this->isDead()) {
        std::cout << name << " succumbs to their afflictions and collapses!\n";
        return; 
    }
    
    std::cout << "\n[" << name << "'s Turn]\n";
    int chosenAction = decideAction();
    
    if (chosenAction == 1) {
        std::cout << name << " darts back and completely disappears into the shadows!\n";
        
        // Give the Goblin +10 Defense for 2 turns!
        StatModifier stealthBuff(DEFENSE, 10, 2);
        this->applyModifier(stealthBuff); 
        
    } else {
        std::cout << name << " bounds forward and slashes at " << target->getName() << "!\n";
        
        int damage = calculateDamage(10); 
        target->takeDamage(damage);
    }
}
void Goblin::serialize(json& j) const {
    Enemy::serialize(j); 
    j["type"] = "Goblin";
    j["stealthChance"] = stealthChance;
}
void Goblin::deserialize(const json& j) {
    Enemy::deserialize(j);
    stealthChance = j.value("stealthChance", 0.1f);
}
