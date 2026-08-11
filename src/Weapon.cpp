#include "Weapon.h"
#include "Player.h"
#include <iostream>
#include <sstream>

Weapon::Weapon(const std::string &name, int baseDamage, int value, int maxDurability)
    : Equippable(name, value, maxDurability), baseDamage(baseDamage) {}

Weapon::Weapon(const Weapon &other)
    : Equippable(other), baseDamage(other.baseDamage) {}

void Weapon::onEquip(Player *target)
{
    if (!target)
        return;
    std::cout << target->getName() << " equipped " << name << " (+" << baseDamage << " ATK)\n";
}

void Weapon::onUnequip(Player *target)
{
    if (!target)
        return;
    std::cout << target->getName() << " unequipped " << name << "\n";
}

void Weapon::onHit()
{
    degrade(1);
}

int Weapon::getBaseDamage() const
{
    return baseDamage;
}

void Weapon::setBaseDamage(int d)
{
    baseDamage = d;
}

Dagger::Dagger(const std::string &n, int dmg, int poison, int val)
    : Weapon(n, dmg, val, 60), poisonChance(poison) {}

Dagger::Dagger(const Dagger &other)
    : Weapon(other), poisonChance(other.poisonChance) {}

void Dagger::onHit()
{
    Weapon::onHit();
    std::cout << "  [Dagger] Poison check (" << poisonChance << "% chance)\n";
}

std::string Dagger::getType() const { return "Dagger"; }
int Dagger::getPoisonChance() const { return poisonChance; }

Sword::Sword(const std::string &n, int dmg, float ls, int val)
    : Weapon(n, dmg, val), lifeStealRatio(ls) {}

Sword::Sword(const Sword &other)
    : Weapon(other), lifeStealRatio(other.lifeStealRatio) {}

void Sword::onEquip(Player *target)
{
    Weapon::onEquip(target);
    std::cout << "  [Sword] Lifesteal: " << (lifeStealRatio * 100.f) << "%\n";
}

std::string Sword::getType() const { return "Sword"; }
float Sword::getLifeStealRatio() const { return lifeStealRatio; }

Axe::Axe(const std::string &n, int dmg, int cleave, int val)
    : Weapon(n, dmg, val, 120), cleaveBonus(cleave) {}

Axe::Axe(const Axe &other)
    : Weapon(other), cleaveBonus(other.cleaveBonus) {}

void Axe::onHit()
{
    Weapon::onHit();
    std::cout << "  [Axe] Cleave for " << cleaveBonus << " to nearby enemies!\n";
}

std::string Axe::getType() const { return "Axe"; }
int Axe::getCleaveBonus() const { return cleaveBonus; }
