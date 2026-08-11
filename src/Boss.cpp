#include "Boss.h"
#include <iostream>

Boss::Boss() {
    name = "The Lich Overlord";
    maxHP = 300;
    currentHP = 300;
    level = 15;
    critRate = 0.25f;
    critMultiplier = 2.5f;
    phase = 1;
    chargeCounter = 0;
}

Boss::~Boss() {}

void Boss::triggerSpecial() {
    std::cout << ">>> " << name << " FIRES ITS ULTIMATE OBLITERATION BEAM! <<<\n";
    chargeCounter = 0; // Reset after using special
    
    // Evaluate if we should change phase based on HP pool thresholds
    if (currentHP <= (maxHP / 2) && phase == 1) {
        phase = 2;
        std::cout << ">>> " << name << " HAS REACHED ENRAGE THRESHOLD. PHASE 2 INITIATED! <<<\n";
        critRate += 0.20f; // Boss hits harder via crits in phase 2
        name = "The Lich Overlord (True Form)";
    }
}

int Boss::decideAction() {
    chargeCounter++;
    
    // In phase 1 requires 3 charges, phase 2 requires only 2 charges
    int requiredCharges = (phase == 1) ? 3 : 2;
    
    if (chargeCounter >= requiredCharges) {
        return 1; // 1 = special nuke trigger
    }
    return 0; // 0 = standard earth-shattering melee
}

void Boss::takeTurn(Entity* target) {
    processStatusEffects(); 

    if (this->isDead()) {
        std::cout << name << " succumbs to their afflictions and collapses!\n";
        return; 
    }
    
    std::cout << "\n[" << name << "'s Turn] [Phase: " << phase << "]\n";
    int chosenAction = decideAction();
    
    if (chosenAction == 1) {
        triggerSpecial(); 
        std::cout << name << " unleashed a devastating special attack!\n";
    } else {
        std::cout << name << " summons jagged earth pillars at " << target->getName() << "! "
                  << "(Charging core: " << chargeCounter << " stacks)\n";
        
        int damage = calculateDamage(15 + (chargeCounter * 5));
        target->takeDamage(damage);
    }
}


void Boss::serialize(json& j) const {
    Enemy::serialize(j); 
    j["type"] = "Boss";
    j["phase"] = phase;
    j["chargeCounter"] = chargeCounter;
}
void Boss::deserialize(const json& j) {
    Enemy::deserialize(j);
    phase = j.value("phase", 1);
    chargeCounter = j.value("chargeCounter", 0);
}