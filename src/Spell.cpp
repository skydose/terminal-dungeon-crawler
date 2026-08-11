#include "Spell.h"
#include "iostream"
#include "Player.h"
#include "Entity.h"

using namespace std;

float Spell::globalManaTax = 0.0f;

Spell::Spell(std::string n, int m) : name(n), manaCost(m) {}

std::string Spell::getName() const
{ return name;
}
int Spell::getManaCost() const
{ return manaCost;
}

HealingSpell::HealingSpell(std::string n, int m, int h) : Spell(n, m), healAmount(h) {}

void HealingSpell::cast(Entity* caster, Entity* target) {
    Player* p = dynamic_cast<Player*>(caster);
    if (p) p->setMana(p->getMana() - manaCost);


    cout << caster->getName() << " is casting Healing Spell: " << name << "!\n";
    effect(caster, target);
}

void HealingSpell::effect(Entity*, Entity* target) {
    cout << name << " targets " << target->getName() << " for a heal of " << healAmount << "!\n";
    target->takeDamage(-healAmount);
}

DamageSpell::DamageSpell(std::string n, int m, int d, bool hasStatus, StatusType sType, int sDur, int sTick) : Spell(n, m), damage(d), causesStatus(hasStatus), statusType(sType), statusDuration(sDur), statusTickValue(sTick) {}

void DamageSpell::cast(Entity* caster, Entity* target) {
    cout << caster->getName() << " is casting Damage Spell: " << name << "!\n";

    Player* p = dynamic_cast<Player*>(caster);
    if (p) p->setMana(p->getMana() - manaCost);

    effect(caster, target);
}

void DamageSpell::effect(Entity* , Entity* target) {
    cout << name << " deals " << damage << " damage to " << target->getName() << "!\n";
    target->takeDamage(damage);

    if (causesStatus) {
        StatusEffect effect(statusType, statusDuration, statusTickValue);
        effect.apply(target);
    }
}