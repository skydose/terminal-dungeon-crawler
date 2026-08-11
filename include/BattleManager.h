#ifndef BATTLEMANAGER_H
#define BATTLEMANAGER_H

#include "DynamicArray.h" // Requires strictly built template headers

#include "Player.h"
#include "Enemy.h"
#include "DynamicArray.h"
#include <string>

class BattleManager {
private:
    static BattleManager* instance;
    // Aggregation: Explicit array management maintaining runtime references natively, NO vector dependencies
    Player* player;
    DynamicArray<Enemy*> enemies;
    bool active;

    BattleManager() {}
    void renderBattleScreen(Player* player, DynamicArray<Enemy*>& enemies, std::string message);

    Enemy* selectTarget();
public:
    ~BattleManager() = default;
    BattleManager(const BattleManager&) = delete;
    BattleManager& operator=(const BattleManager&) = delete;

    static BattleManager* getInstance();
    
    Enemy* selectTarget(DynamicArray<Enemy*>& enemies);
    // Preserves API parity for 1v1 legacy logic by implicitly array-wrapping the standalone parameter
    void startBattle(Player* player, DynamicArray<Enemy*>& enemies);
    
    // Aggregate State Extractors
    Player* getPrimaryPlayer() const { return player; }
    void setPrimaryPlayer(Player* p) { player = p; }

    bool isBattleActive() const { return active; }
    void setBattleActive(bool state) { active = state; }

    DynamicArray<Enemy*>& getCombatants() { return enemies; }

    void processTurn();
    int calculateFinalDamage(Entity* attacker, Entity* target);
};

#endif // BATTLEMANAGER_H