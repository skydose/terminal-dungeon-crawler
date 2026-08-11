#include "Armor.h"

Armor::Armor(const std::string &n, int def, int val, int maxDur)
    : Equippable(n, val, maxDur), baseDefense(def) {}

Armor::Armor(const Armor &other)
    : Equippable(other), baseDefense(other.baseDefense) {}

void Armor::onEquip(Player *target)
{
    if (!target)
        return;
    std::cout << target->getName() << " equipped " << name << " (+" << baseDefense << " DEF)\n";
}

void Armor::onUnequip(Player *target)
{
    if (!target)
        return;
    std::cout << target->getName() << " unequipped " << name << "\n";
}

void Armor::onHit() { degrade(1); }

std::string Armor::getType() const { return "Armor"; }
int Armor::getBaseDefense() const { return baseDefense; }
void Armor::setBaseDefense(int d) { baseDefense = d; }


PlateArmor::PlateArmor(const std::string &n, int def, float penalty, int val)
    : Armor(n, def, val, 150), weightPenalty(penalty) {}

PlateArmor::PlateArmor(const PlateArmor &other)
    : Armor(other), weightPenalty(other.weightPenalty) {}

void PlateArmor::onEquip(Player *target)
{
    Armor::onEquip(target);
    std::cout << "  [PlateArmor] Speed reduced by " << (weightPenalty * 100.f) << "%\n";
}

std::string PlateArmor::getType() const { return "PlateArmor"; }
float PlateArmor::getWeightPenalty() const { return weightPenalty; }

ChainMail::ChainMail(const std::string &n, int def, int rings, int val)
    : Armor(n, def, val, 90), linkedRings(rings) {}

ChainMail::ChainMail(const ChainMail &other)
    : Armor(other), linkedRings(other.linkedRings) {}

void ChainMail::onEquip(Player *target)
{
    Armor::onEquip(target);
    std::cout << "  [ChainMail] " << linkedRings << " linked rings.\n";
}

std::string ChainMail::getType() const { return "ChainMail"; }
int ChainMail::getLinkedRings() const { return linkedRings; }
