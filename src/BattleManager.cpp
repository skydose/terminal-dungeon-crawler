#include "BattleManager.h"
#include "Inventory.h"
#include "Equippable.h" 
#include "colorcodes.h"
#include <iostream>
#include "Spellbook.h"
#include "InputManager.h"
#include "SkillManager.h" 
#include "Skill.h"       

using namespace std;

BattleManager* BattleManager::instance = nullptr;

BattleManager* BattleManager::getInstance() {
    if (instance == nullptr) {
        instance = new BattleManager();
    }
    return instance;
}

void BattleManager::renderBattleScreen(Player* player, DynamicArray<Enemy*>& enemies, string message) {
    cout << "\033[2J\033[1;1H"; // ansi clear screen

    cout << RED << "======================================================\n" << RESET;
    cout << RED << "||" << RESET << "                    COMBAT ENGAGED                  " << RED << "||\n" << RESET;
    cout << RED << "======================================================\n\n" << RESET;

    for (int i = 0; i < enemies.size(); i++) {
        if (!enemies[i]->isDead()) {
            cout << MAGENTA << "  [" << (i + 1) << "] " << enemies[i]->getName() << RESET << "\n";
            cout << "      HP: " << enemies[i]->getHealthBar(15, RED) 
                 << " " << enemies[i]->getHP() << "/" << enemies[i]->getMaxHP() << "\n\n";
        } else {
            cout << GRAY << "  [X] " << enemies[i]->getName() << " (DEFEATED)\n\n" << RESET;
        }
    }

    cout << CYAN << "------------------------- VS -------------------------\n\n" << RESET;

    cout << GREEN << "      " << player->getName() << RESET << "\n";
    cout << "      HP: " << player->getHealthBar(20, GREEN) 
         << " " << player->getHP() << "/" << player->getMaxHP() << "\n";
    cout << "      MP: " << BLUE << player->getMana() << "/" << player->getMaxMana() << RESET << "\n\n";

    cout << RED << "======================================================\n" << RESET;
    
    if (message != "") {
        cout << YELLOW << " > " << message << "\n" << RESET;
        cout << RED << "======================================================\n" << RESET;
    }

    cout << " [1] Attack          [2] Spells \n";
    cout << " [3] Inventory       [4] Flee \n";
    cout << RED << "======================================================\n" << RESET;
}

// Target Selection Helper
Enemy* BattleManager::selectTarget(DynamicArray<Enemy*>& enemies) {
    cout << "\n--- SELECT TARGET ---\n";
    for (int i = 0; i < enemies.size(); i++) {
        if (!enemies[i]->isDead()) {
            cout << "[" << (i + 1) << "] " << enemies[i]->getName() << " (" << enemies[i]->getHP() << " HP)\n";
        }
    }
    cout << "[0] Cancel\nChoice: ";
    
    int t;
    cin >> t;
    if (cin.fail()) {
        cin.clear(); cin.ignore(10000, '\n'); return nullptr;
    }
    cin.ignore(10000, '\n');

    if (t > 0 && t <= enemies.size() && !enemies[t-1]->isDead()) {
        return enemies[t-1];
    }
    return nullptr;
}


void BattleManager::startBattle(Player* player, DynamicArray<Enemy*>& enemies) {
    player->clearModifiers(); 

    bool hasPassives = false;
    
    for(int i = 0; i < player->getNumUnlockedSkills(); i++) {
        Skill* passiveSkill = player->getUnlockedSkill(i);
        
        if(passiveSkill != nullptr) {
            passiveSkill->activate(player);
            hasPassives = true;
        }
    }
    
    if (hasPassives) {
        cout << "\nPress Enter to begin the battle...";
        cin.get();
    }

    bool inBattle = true;
    string battleMessage = "Enemies are preparing to strike!";

    cin.clear();

    while (inBattle && !player->isDead()) {
        
        bool allDead = true;
        for (int i = 0; i < enemies.size(); i++) {
            if (!enemies[i]->isDead()) allDead = false;
        }
        if (allDead) {
            renderBattleScreen(player, enemies, "Victory! All enemies defeated.");
            player->clearModifiers(); // PASSIVE CLEANUP
            cout << "\nPress Enter to continue...";
            cin.get();
            return;
        }

        // player's turn
        renderBattleScreen(player, enemies, battleMessage);
        battleMessage = "";

        cout << "\nCommand: ";
        char input = InputManager::getChar();

        bool turnConsumed = false;

        switch(input) {
            case '1': { // ATTACK
                Enemy* target = selectTarget(enemies);
                if (target != nullptr) {
                    player->attack(target);
                    battleMessage = "You struck " + target->getName() + "!";
                    turnConsumed = true;
                } else {
                    battleMessage = "Attack cancelled.";
                }
                break;
            }
            case '2': { // SPELLS
                Spellbook* spellbook = player->getSpellbook();
                if (spellbook == nullptr) {
                    battleMessage = "You don't own a Mystic Grimoire!";
                    break;
                }

                spellbook->display(); 
                
                cout << "\n [0] Cancel | Select Spell Number: ";
                int spellIdx;
                cin >> spellIdx;

                if (cin.fail()) {
                    cin.clear(); cin.ignore(10000, '\n');
                    battleMessage = "Invalid input.";
                    break;
                }
                cin.ignore(10000, '\n');

                if (spellIdx > 0) {
                    Spell* selectedSpell = spellbook->getSpell(spellIdx - 1);
                    
                    if (selectedSpell != nullptr) {
                        
                        if (player->getMana() < selectedSpell->getManaCost()) {
                            battleMessage = "Not enough MP to cast " + selectedSpell->getName() + "!";
                        } 
                        else {
                            HealingSpell* heal = dynamic_cast<HealingSpell*>(selectedSpell);
                            
                            if (heal != nullptr) {
                                selectedSpell->cast(player, player); // Caster: Player, Target: Player
                                battleMessage = "You cast " + selectedSpell->getName() + "!";
                                turnConsumed = true;
                            } 
                            else {
                                Enemy* target = selectTarget(enemies);
                                if (target != nullptr) {
                                    selectedSpell->cast(player, target);
                                    battleMessage = "You blasted " + target->getName() + " with " + selectedSpell->getName() + "!";
                                    turnConsumed = true;
                                } else {
                                    battleMessage = "Spell cancelled.";
                                }
                            }
                        }
                    } else {
                        battleMessage = "Invalid spell selection.";
                    }
                } else {
                    battleMessage = "Grimoire closed.";
                }
                break;
            }
            
            case '3': { // EQUIPMENT
                Inventory* inv = player->getInventory();
                inv->display(); 
                
                cout << "\n [0] Cancel | Select Item Number: ";
                int itemIdx;
                cin >> itemIdx;

                if (cin.fail()) {
                    cin.clear(); cin.ignore(10000, '\n');
                    battleMessage = "Invalid input.";
                    break;
                }
                cin.ignore(10000, '\n');

                if (itemIdx > 0 && itemIdx <= inv->getNumItems()) {
                    Item* selected = inv->getItem(itemIdx - 1);
                    
                    Equippable* gear = dynamic_cast<Equippable*>(selected);
                    
                    if (gear != nullptr) {
                        player->equip(gear);
                        battleMessage = "Equipped " + gear->getName() + "!";
                        turnConsumed = true;
                    } 
                    else {
                        selected->use(player);
                        battleMessage = "Used " + selected->getName() + "!";
                        inv->removeItem(itemIdx - 1); // Remove the consumed item
                        turnConsumed = true;
                    }
                } else {
                    battleMessage = "Bag closed.";
                }
                break;
            }
            case '4': // FLEE
                cout << "\n" << YELLOW << "You turned and fled from the battle!" << RESET << "\n";
                player->clearModifiers(); // PASSIVE CLEANUP
                cout << "Press Enter to return to the room...";
                cin.get();
                return;

            default:
                battleMessage = "You hesitated...";
                break;
        }

        // enemy's turn
        if (turnConsumed) {
            for (int i = 0; i < enemies.size(); i++) {
                if (!enemies[i]->isDead() && !player->isDead()) {
                    
                    // --- APPLY BURN/POISON DAMAGE FIRST ---
                    enemies[i]->processStatusEffects();

                    // --- ONLY ATTACK IF THE BURN DIDN'T KILL THEM! ---
                    if (!enemies[i]->isDead()) {
                        enemies[i]->takeTurn(player); 
                        battleMessage += " " + enemies[i]->getName() + " retaliated!";
                    }
                }
            }
        }
    }

    // in case you die
    if (player->isDead()) {
        player->clearModifiers(); // PASSIVE CLEANUP
        cout << "\033[2J\033[1;1H"; 
        cout << RED << "\n\n        ======================\n";
        cout << "        ====== YOU DIED ======\n";
        cout << "        ======================\n\n" << RESET;
        cout << "Press Enter to face your fate...";
        cin.get();
    }
}