#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"
#include "Serializable.h"


class LootTable;
class LootBundle;
class StatusEffect;
class StatModifier;

class Enemy : public Entity {
protected:
    LootTable* lootTable;
    StatusEffect* activeEffects;
    int numEffects;
    int effectCapacity;

    StatModifier* activeModifiers;
    int numModifiers;
    int modifierCapacity;

public:
    Enemy();
    virtual ~Enemy() override;

    // Standard Accessors
    LootTable* getLootTable() const { return lootTable; }
    void setLootTable(LootTable* lt) { lootTable = lt; }
    
    StatusEffect* getActiveEffects() const { return activeEffects; }
    void setActiveEffects(StatusEffect* effects) { activeEffects = effects; }

    int getNumEffects() const { return numEffects; }
    void setNumEffects(int num) { numEffects = num; }

    int getEffectCapacity() const { return effectCapacity; }
    void setEffectCapacity(int capacity) { effectCapacity = capacity; }

    virtual int decideAction() = 0;
    void takeTurn(Entity* target) override = 0;
    LootBundle dropLoot();

    void applyModifier(StatModifier mod);
    void takeDamage(int amount);

    void addStatusEffect(StatusEffect effect);
    void processStatusEffects(); // we have to call this at the start of every turn

    void serialize(json& j) const override;
    void deserialize(const json& j) override;
};

#endif // ENEMY_H