#include <iostream>
#include <cstdlib>
#include <ctime>

// Include the headers from our newly created combat cluster
#include "Armor.h"
#include "BattleManager.h"
#include "Boss.h"
#include "colorcodes.h"
#include "CombatRoom.h"
#include "Consumable.h"
#include "CombatRules.h"
#include "Coordinate.h"
#include "DynamicArray.h"
#include "EmptyRoom.h"
#include "Enemy.h"
#include "Entity.h"
#include "Equippable.h"
#include "GameState.h"
#include "Goblin.h"
#include "Inventory.h"
#include "Item.h"
#include "ItemRules.h"
#include "json.hpp"
#include "Knight.h"
#include "LootTable.h"
#include "MagicRules.h"
#include "MapGrid.h"
#include "Player.h"
#include "Room.h"
#include "SaveRoom.h"
#include "Skill.h"
#include "SkillManager.h"
#include "SkillRules.h"
#include "Spell.h"
#include "Spellbook.h"
#include "StatModifier.h"
#include "StatusEffect.h"
#include "Store.h"
#include "Weapon.h"
#include "WorldGenerator.h"
#include "InputManager.h"
#include "WorldRules.h"
#include <filesystem> 
#include <string>

using namespace std;
namespace fs = std::filesystem;

// ==========================================
// MENU HELPER FUNCTIONS
// ==========================================
void handleSaveMenu(GameState* gs) {
    cout << MAGENTA << "\n=== SAVE GAME ===\n" << RESET;
    
    for (int i = 1; i <= 3; ++i) {
        string filename = "saves/save" + to_string(i) + ".json";
        if (fs::exists(filename)) {
            cout << " [" << i << "] Save Slot " << i << YELLOW << " (Overwrite)" << RESET << "\n";
        } else {
            cout << " [" << i << "] Save Slot " << i << GREEN << " (Empty)" << RESET << "\n";
        }
    }
    cout << " [0] Cancel\n";
    cout << "Choice: ";
    
    int choice;
    cin >> choice;

    if (cin.fail()) {
        cin.clear(); cin.ignore(10000, '\n');
        cout << RED << "Invalid input. Save cancelled.\n" << RESET;
        return;
    }
    cin.ignore(10000, '\n');

    if (choice >= 1 && choice <= 3) {
        gs->save(choice); 
        cout << GREEN << ">>> Game Progress Successfully Saved in Slot " << choice << "! <<<\n" << RESET;
    } else if (choice != 0) {
        cout << RED << "Invalid slot. Save cancelled.\n" << RESET;
    }
}

bool handleLoadMenu(GameState* gs) {
    cout << MAGENTA << "\n=== LOAD GAME ===\n" << RESET;
    bool saveExists[3] = {false, false, false};

    for (int i = 1; i <= 3; ++i) {
        string filename = "saves/save" + to_string(i) + ".json";
        if (fs::exists(filename)) {
            cout << " [" << i << "] Save Slot " << i << GREEN << " (Available)" << RESET << "\n";
            saveExists[i - 1] = true;
        } else {
            cout << " [" << i << "] Save Slot " << i << YELLOW << " (Empty)" << RESET << "\n";
        }
    }
    cout << " [0] Cancel\n";
    cout << "Choice: ";
    
    int choice;
    cin >> choice;

    if (cin.fail()) {
        cin.clear(); cin.ignore(10000, '\n');
        cout << RED << "Invalid input.\n" << RESET;
        return false;
    }
    cin.ignore(10000, '\n');

    if (choice >= 1 && choice <= 3) {
        if (saveExists[choice - 1]) {
            if (gs->load(choice)) {
                return true;
            }
        } else {
            cout << RED << "That slot is empty!\n" << RESET;
        }
    } else if (choice != 0) {
        cout << RED << "Invalid choice.\n" << RESET;
    }
    return false;
}

// ==========================================
// GAME LOOPS
// ==========================================
void playGame() {
    GameState* gs = GameState::getInstance();
    Player* player = gs->getPlayer();
    MapGrid* dungeon = gs->getMap();

    bool gameRunning = true;
    char input;

    while (gameRunning && !player->isDead()) {
        dungeon->render(); 
        
        cout << "\nWhat is your move, " << player->getName() << "?\n";
        cout << "[W] Up | [S] Down | [A] Left | [D] Right\n";
        cout << "[Z] Save Game | [Q] Return to Main Menu\n";
        cout << "Command: ";
        
        input = InputManager::getChar();
        input = toupper(input);

        if (input == 'Z') {
            handleSaveMenu(gs); // Hooked in the new save menu
            cout << "Press Enter to continue...";
            cin.get();
            continue;
        }
        else if (input == 'Q') {
            cout << YELLOW << "Returning to the Main Menu...\n" << RESET;
            gameRunning = false;
            continue;
        }

        int dx = 0, dy = 0;
        if (input == 'W') dy = -1;
        else if (input == 'S') dy = 1;
        else if (input == 'A') dx = -1;
        else if (input == 'D') dx = 1;
        else {
            cout << RED << "Invalid command! Press Enter to try again." << RESET;
            cin.ignore(10000, '\n'); cin.get();
            continue;
        }

        dungeon->move(dx, dy, player);
        
        cout << "\nPress Enter to continue..." << endl;
        cin.ignore(10000, '\n'); cin.get();
    }
    
    gs->cleanup(); 
}

int main() {
    WorldRules::worldSeed = time(0);
    bool mainRunning = true;

    while (mainRunning) {
        cout << "\033[2J\033[1;1H"; // Clear Screen
        cout << MAGENTA << "=======================================\n" << RESET;
        cout << MAGENTA << "||" << RESET << YELLOW << "           DUNGEON RPG             " << MAGENTA << "||\n" << RESET;
        cout << MAGENTA << "=======================================\n" << RESET;
        cout << " [1] New Game\n";
        cout << " [2] Load Game\n";
        cout << " [3] Exit to Desktop\n";
        cout << MAGENTA << "=======================================\n" << RESET;
        cout << "Choice: ";
        
        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); cin.ignore(10000, '\n'); continue;
        }
        cin.ignore(10000, '\n');

        GameState* gs = GameState::getInstance();

        if (choice == 1) {
            cout << "\033[2J\033[1;1H"; // Clear Screen
            cout << MAGENTA << "=======================================\n" << RESET;
            cout << MAGENTA << "||" << RESET << YELLOW << "            THE BEGINNING            " << MAGENTA << "||\n" << RESET;
            cout << MAGENTA << "=======================================\n\n" << RESET;
            
            cout  << BOLD << "*You open your eyes- after looking around you realize you're in an unfamiliar place..?\n" << RESET;
            InputManager::pause();

            cout << "\n" << BLUE << "You: " << RESET << BOLD << " \"Am I dreaming? Last thing I remember was struggling with my uni's awful schedule-\"\n" << RESET;
            InputManager::pause();
            cout << "\n" << BLUE << "You: " << RESET << BOLD << " \"But hey! Being in danger in this weird place is miles better than whatever I was doing!\"\n" << RESET;

            InputManager::pause();
            cout << RED << BOLD << "\n*A voice whispers in your mind: 'Find the Lich.'\n" << RESET;
            InputManager::pause();

            cout << "\n" << BLUE << "You: " << RESET << BOLD << " \"The Lich? Isn't that like from Adventure Time or something? I guess that's my cue to move-\"\n" << RESET;
            InputManager::pause();


            // setting up the skills
            SkillManager* sm = SkillManager::getInstance();
            sm->clearSkills(); 

            ScalingSkill* defenseUp = new ScalingSkill("DEFENSE UP +1", 2, 1.0f, StatModifier(DEFENSE, 5)); 
            sm->learnSkill(defenseUp); 

            ScalingSkill* defenseUp2 = new ScalingSkill("DEFENSE UP +2", 5, 1.5f, StatModifier(DEFENSE, 10)); 
            sm->learnSkill(defenseUp2); 

            ScalingSkill* attackUp1 = new ScalingSkill("ATTACK UP +1", 3, 1.0f, StatModifier(DAMAGE, 10));
            sm->learnSkill(attackUp1); 

            ScalingSkill* attackUp2 = new ScalingSkill("ATTACK UP +2", 5, 1.5f, StatModifier(DAMAGE, 25));
            sm->learnSkill(attackUp2); 

            gs->setPlayer(new Player());
            gs->setMap(WorldGenerator::getInstance()->generateMap(WorldRules::mapHeight, WorldRules::mapWidth));
            
            playGame();
        } 
        else if (choice == 2) {
            
            gs->setPlayer(new Player());
            gs->setMap(new MapGrid(WorldRules::mapHeight, WorldRules::mapWidth));

            // Hooked in the new load menu
            if (handleLoadMenu(gs)) {
                cout << YELLOW << "\nGame successfully loaded!\n" << RESET;
                playGame();
            } else {
                cout << "\nPress Enter to return to menu.";
                gs->cleanup(); 
                cin.get();
            }
        } 
        else if (choice == 3) {
            cout << "Thank you for playing the game!\n";
            mainRunning = false;
        }
    }

    return 0;
}