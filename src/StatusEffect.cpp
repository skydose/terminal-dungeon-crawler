#include "StatusEffect.h"
#include <iostream>
#include "Enemy.h"
using namespace std;

StatusEffect::StatusEffect() : type(BURN), duration(0), tickValue(0) {}
StatusEffect::StatusEffect(StatusType t, int d, int tv) : type(t), duration(d), tickValue(tv) {}

void StatusEffect::apply(Entity* target) {
    if (!target) return;
    
    cout << "A lingering effect attaches to " << target->getName() << " for " << duration << " rounds.\n";
    
    // Safely attempt to cast the target to an Enemy
    Enemy* enemyTarget = dynamic_cast<Enemy*>(target);
    if (enemyTarget) {
        
        enemyTarget->addStatusEffect(*this); 
    }
}

void StatusEffect::tick(Entity* target) {
if (duration > 0)
{
int damagePerTick = 5;
cout << target->getName() << " suffers from a status effect this round!\n";

// Loop 'tickValue' times per round
for(int i = 0; i < tickValue; ++i) {
switch(type) {
case POISON:
    cout<<"  -> POISON hit! Takes "<<damagePerTick<< " damage.\n";
    target->takeDamage(damagePerTick);
    break;
case BURN:
    cout << "  -> BURN hit! Takes " << damagePerTick << " damage.\n";
    target->takeDamage(damagePerTick);
    break;
case FREEZE:
    cout << "  -> FREEZE hit! Takes " << damagePerTick << " cold damage.\n";
    target->takeDamage(damagePerTick);
    break;
}
}
duration--; // Reduce the round count by 1
}
}