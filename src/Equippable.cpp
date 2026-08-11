#include "Equippable.h"
#include <iostream>

Equippable::Equippable(const std::string &n, int v, int maxDur)
    : Item(n, v), durability(maxDur), maxDurability(maxDur) {}

Equippable::Equippable(const Equippable &other)
    : Item(other), durability(other.durability), maxDurability(other.maxDurability) {}

int Equippable::getDurability() const { return durability; }
int Equippable::getMaxDurability() const { return maxDurability; }
bool Equippable::isBroken() const { return durability <= 0; }

void Equippable::repair(int amount)
{
    durability += amount;
    if (durability > maxDurability)
        durability = maxDurability;
}

void Equippable::degrade(int amount)
{
    durability -= amount;
    if (durability < 0)
        durability = 0;
    if (isBroken())
        std::cout << "[WARNING] " << name << " has broken!\n";
}


