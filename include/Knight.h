#ifndef KNIGHT_H
#define KNIGHT_H

#include "Enemy.h"

class Knight : public Enemy {
private:
    float blockChance;

public:
    Knight();
    virtual ~Knight() override;

    // Parameter Accessors
    float getBlockChance() const { return blockChance; }
    void setBlockChance(float chance) { blockChance = chance; }

    int decideAction() override;
    void takeTurn(Entity* target) override;

    void serialize(json& j) const override;
    void deserialize(const json& j) override;
};

#endif // KNIGHT_H