#ifndef STATUSEFFECT_H
#define STATUSEFFECT_H
#include "Entity.h"

enum StatusType { POISON, BURN, FREEZE };

class StatusEffect {
private:
    StatusType type;
    int tickValue;

public:
    int duration;
    StatusEffect();
    StatusEffect(StatusType t, int d, int tv);
    void apply(Entity* target);
    void tick(Entity* target);

    int getDuration() const { return duration; }
    StatusType getType() const { return type; }
};

#endif