#include "Spellbook.h"
#include <iostream>
#include "colorcodes.h"
using namespace std;

Spellbook::Spellbook() {
}

Spellbook::~Spellbook() {
    for (int i = 0; i < spells.size(); i++) {
        if (spells[i]) {
            delete spells[i];
        }
    }
}

void Spellbook::addSpell(Spell* s) {
    // Edge Case, dont let a null pointer crash the array
    if (s == nullptr)
{
    cout << "Error: Cannot add an empty spell to the Spellbook.\n";
    return;
    }
    spells.push_back(s);
}

void Spellbook::removeSpell(int index)
{
    spells.removeAt(index);
}

void Spellbook::viewSpells() const {
    // Edge Case, its for the empty inventory check
    if (spells.size() == 0) {
    cout << "Your Spellbook is currently empty.\n";
    return;
}

    cout << "\n--- Your Spellbook ---\n";
    for (int i = 0; i < spells.size(); i++) {
        cout << i+1 << ". " << spells[i] << "\n";
    }
}

void Spellbook::serialize(json &j) const {
    json spellsArray = json::array();

    for(int i = 0; i < spells.size(); i++) {
        if(spells[i]) {
            json spell;
            spells[i]->serialize(spell);
            spellsArray.push_back(spell);
        }
    }
    j["spells"] = spellsArray;
}

void Spellbook::deserialize(const json& j) {
    for(int i = 0; i < spells.size(); i++) delete spells[i];
    while(spells.size() > 0) { spells.removeAt(0); }

    if(j.contains("spells")) {
        for(const auto& jsonSpells : j["spells"]) {
            string type = jsonSpells.value("type", "Unknown");
            Spell* spell = nullptr;

            if (type == "DamageSpell") spell = new DamageSpell();
            else if (type == "HealingSpell") spell = new HealingSpell();
            
            if(spell) {
                spell->deserialize(jsonSpells);
                addSpell(spell);
            }

        }
    }

}


void Spellbook::display() const {
    cout << "\033[2J\033[1;1H"; // ANSI Clear Screen
    
    cout << CYAN << "╔═══════════════════════════════════════╗\n" << RESET;
    cout << CYAN << "║" << RESET << MAGENTA << "            MYSTIC GRIMOIRE            " << CYAN << "║\n" << RESET;
    cout << CYAN << "╠═══════════════════════════════════════╣\n" << RESET;
    
    if (spells.size() == 0) {
        cout << CYAN << "║" << RESET << GRAY << "       Your spellbook is empty...      " << RESET << CYAN << "║\n" << RESET;
    } else {
        for (int i = 0; i < spells.size(); ++i) {
            cout << CYAN << "║ " << RESET;
            
            // Add an extra space for single-digit numbers to keep brackets aligned
            if (i + 1 < 10) cout << " ";
            
            cout << YELLOW << "[" << (i + 1) << "] " << RESET;
            
            string spellName = spells[i]->getName();
            string costStr = "(" + to_string(spells[i]->getManaCost()) + " MP)";
            
            cout << spellName;
            
            // Math to perfectly align the MP cost to the right wall
            int padding = 32 - spellName.length() - costStr.length();
            for(int p = 0; p < padding; p++) {
                cout << " ";
            }
            
            // Print the Mana cost in Blue!
            cout << BLUE << costStr << RESET << CYAN << " ║\n" << RESET;
        }
    }
    cout << CYAN << "╚═══════════════════════════════════════╝\n" << RESET;
}