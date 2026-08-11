#ifndef GOBLIN_H
#define GOBLIN_H

#include "Enemy.h"

class Goblin : public Enemy {
private:
    float stealthChance;

public:
    Goblin();
    virtual ~Goblin() override;

    // Parameter Accessors
    float getStealthChance() const { return stealthChance; }
    void setStealthChance(float chance) { stealthChance = chance; }

    int decideAction() override;
    void takeTurn(Entity* target = nullptr) override;

    void serialize(json& j) const override;
    void deserialize(const json& j) override;
};

#endif // GOBLIN_H