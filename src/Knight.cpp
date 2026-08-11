#include "Knight.h"
#include <iostream>
#include <cstdlib>
#include "Weapon.h"
#include "Consumable.h"
#include "Armor.h"

#include "LootTable.h"


Knight::Knight() {
    name = "Knight";
    maxHP = 90;
    currentHP = 90;
    level = 5;
    critRate = 0.1f;
    critMultiplier = 1.5f;
    blockChance = 0.45f;
    lootTable = new LootTable(6); 
    lootTable->setXPRange(10, 20);
    lootTable->setGoldRange(5, 12);

    lootTable->addEntry(new Sword("Iron Sword", 45, 18), 0.30f); 
    lootTable->addEntry(new Axe("Steel Battleaxe", 60, 22), 0.15f);   
    lootTable->addEntry(new ChainMail(), 0.15f); 
    lootTable->addEntry(new HealthPotion(50, 25), 0.15f);             
    lootTable->addEntry(new DamagePotion(25, 35), 0.15f);
    lootTable->addEntry(new PlateArmor(), 0.1f);
}

Knight::~Knight() {}

int Knight::decideAction() {
    float randVal = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    if (randVal <= blockChance) {
        return 1; // 1 = heavy block
    }
    return 0; // 0 = standard sword swing
}

void Knight::takeTurn(Entity* target) {
    processStatusEffects(); 

    if (this->isDead()) {
        std::cout << name << " succumbs to their afflictions and collapses!\n";
        return; 
    }

    std::cout << "\n[" << name << "'s Turn]\n";
    int chosenAction = decideAction();
    
    if (chosenAction == 1) {
        std::cout << name << " firmly plants its boots and hoists a massive tower shield. Block initiated!\n";
    } else {
        std::cout << name << " steps deeply into a sweeping arc, attempting a catastrophic slash at " 
                  << target->getName() << "!\n";
        
        int damage = calculateDamage(20);
        target->takeDamage(damage);
    }
}

void Knight::serialize(json& j) const {
    Enemy::serialize(j); 
    j["type"] = "Knight";
    j["blockChance"] = blockChance;
}
void Knight::deserialize(const json& j) {
    Enemy::deserialize(j);
    blockChance = j.value("blockChance", 0.2f);
}
