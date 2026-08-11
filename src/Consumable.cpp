#include "Consumable.h"
#include "StatModifier.h"


Consumable::Consumable(const std::string &n, int v)
    : Item(n, v) {}

Consumable::Consumable(const Consumable &other)
    : Item(other) {}

    
HealthPotion::HealthPotion(int healAmt, int val)
    : Consumable("Health Potion", val), healAmount(healAmt) {}

HealthPotion::HealthPotion(const HealthPotion &other)
    : Consumable(other), healAmount(other.healAmount) {}

void HealthPotion::use(Player *target)
{
    if (!target)
        return;
    int missing = target->getMaxHP() - target->getHP();
    int restored = (healAmount < missing) ? healAmount : missing;
    target->healHP(restored);
    std::cout << "[HealthPotion] " << target->getName()
              << " restored " << restored << " HP"
              << " (" << target->getHP() << "/" << target->getMaxHP() << ")\n";
}

std::string HealthPotion::getType() const { return "HealthPotion"; }
int HealthPotion::getHealAmount() const { return healAmount; }

DamagePotion::DamagePotion(int dmgBuff, int val)
    : Consumable("Damage Potion", val), damageBuffAmount(dmgBuff) {}

DamagePotion::DamagePotion(const DamagePotion &other)
    : Consumable(other), damageBuffAmount(other.damageBuffAmount) {}

void DamagePotion::use(Player *target)
{
    if (!target)
        return;
    StatModifier dmgBuff(DAMAGE, damageBuffAmount, 3);

    target->applyModifier(dmgBuff);
    std::cout << "[DamagePotion] " << target->getName()
              << " gained +" << damageBuffAmount << " damage.\n";
}

std::string DamagePotion::getType() const { return "DamagePotion"; }
int DamagePotion::getDamageBuffAmount() const { return damageBuffAmount; }

ManaPotion::ManaPotion(int restoreAmt, int val)
    : Consumable("Mana Potion", val), restoreAmount(restoreAmt) {}

ManaPotion::ManaPotion(const ManaPotion &other)
    : Consumable(other), restoreAmount(other.restoreAmount) {}

// Mana is managed by Player directly via applyBuff in the full project.
// Here we call applyBuff with 0 damage so it's wired through the real interface.
void ManaPotion::use(Player *target)
{
    if (!target)
        return;
    // ManaPotion restores mana — applyBuff signals the player system.
    // The restoreAmount is passed as the damageMod so the Player
    // can internally route it to mana. Replace with a proper restoreMana()
    // method when the full Player is integrated.
    target->restoreMana(restoreAmount);
    std::cout << "[ManaPotion] " << target->getName()
              << " restored " << restoreAmount << " Mana.\n";
}

std::string ManaPotion::getType() const { return "ManaPotion"; }
int ManaPotion::getRestoreAmount() const { return restoreAmount; }


DefensePotion::DefensePotion(int defBuff, int val)
    : Consumable("Defense Potion", val), defenseBuffAmount(defBuff) {}

DefensePotion::DefensePotion(const DefensePotion &other)
    : Consumable(other), defenseBuffAmount(other.defenseBuffAmount) {}

void DefensePotion::use(Player *target)
{
    if (!target)
        return;
        
    StatModifier defBuff(DEFENSE, defenseBuffAmount, 3); 
    
    target->applyModifier(defBuff);
    
    std::cout << "[DefensePotion] " << target->getName() << " gained +" << defenseBuffAmount << " defense.\n";
}

std::string DefensePotion::getType() const { return "DefensePotion"; }
int DefensePotion::getDefenseBuffAmount() const { return defenseBuffAmount; }
