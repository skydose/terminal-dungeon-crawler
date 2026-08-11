#ifndef BOSS_H
#define BOSS_H

#include "Enemy.h"

class Boss : public Enemy {
private:
    int phase;
    int chargeCounter;

public:
    Boss();
    virtual ~Boss() override;

    void triggerSpecial();
    int decideAction() override;
    void takeTurn(Entity* target) override;
    
    // Standard Accessors
    int getPhase() const { return phase; }
    void setPhase(int p) { phase = p; }

    int getChargeCounter() const { return chargeCounter; }
    void setChargeCounter(int c) { chargeCounter = c; }

    void serialize(json& j) const override;
    void deserialize(const json& j) override;
};

#endif 