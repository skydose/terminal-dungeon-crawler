#include "SkillManager.h"
#include "Skill.h"
#include "DynamicArray.h"
#include "Weapon.h"
#include "Armor.h"
#include "Inventory.h"
#include "Spellbook.h"
#include "StatModifier.h"
#include "Equippable.h"
#include "colorcodes.h"
#include "Consumable.h"



Player::Player() {
    name = "Hero";
    maxHP = 150;
    currentHP = 150;
    level = 1;
    critRate = 0.15f;
    critMultiplier = 1.8f;
    xp = 0;
    gold = 150; // starting gold
    mana = 100;

    inventory = new Inventory(10);

    spellbook = nullptr;
    modifierCapacity = 5;
    equippedWeapon = nullptr;
    equippedArmor = nullptr;
    hpBar.update(currentHP, maxHP);

    skillCapacity = 10; 
}

Player::~Player() {
    if (inventory != nullptr) delete inventory;
    if (spellbook != nullptr) delete spellbook;
    if (equippedWeapon != nullptr) delete equippedWeapon;
    if (equippedArmor != nullptr) delete equippedArmor;
}


void Player::addXP(int x) {
    xp += x;
    
    int xpNeeded = level * 30; 
    
    while (xp >= xpNeeded) {
        xp -= xpNeeded;
        level++;
        maxHP += 20; 
        currentHP = maxHP; 
        
        std::cout << YELLOW << "\n*** LEVEL UP! " << name << " reached Level " << level << "! ***\n" << RESET;
        
        SkillManager* sm = SkillManager::getInstance();
        for(int i = 0; i < sm->getNumSkills(); i++) {
            Skill* skill = sm->getSkill(i);
            
            if(skill != nullptr && skill->getRequiredLevel() == level) {
                this->addSkill(skill); 
                cout << MAGENTA << ">>> NEW PASSIVE UNLOCKED: " << skill->getName() << " <<<\n" << RESET;
            }
        }

        if (spellbook != nullptr) {
            if (level == 2) {
                spellbook->addSpell(new HealingSpell("Lesser Heal", 20, 40));
                std::cout << CYAN << ">>> SPELL UNLOCKED: Lesser Heal!\n" << RESET;
            } else if (level == 3) {
                spellbook->addSpell(new DamageSpell("Fireball", 30, 50));
                std::cout << CYAN << ">>> SPELL UNLOCKED: Fireball!\n" << RESET;
            } else if (level == 4) {
                spellbook->addSpell(new HealingSpell("Great Heal", 40, 100));
                std::cout << CYAN << ">>> SPELL UNLOCKED: Great Heal!\n" << RESET;
            } else if (level == 5) {
                spellbook->addSpell(new DamageSpell("Lightning Storm", 50, 80));
                std::cout << CYAN << ">>> SPELL UNLOCKED: Lightning Storm!\n" << RESET;
            }
        }
        xpNeeded = level * 30; 
    }
}

void Player::attack(Entity* target) {
    if (!target || target->isDead()) return;
    
    int baseDmg = 5; 
    
    if (equippedWeapon != nullptr) {
        baseDmg = equippedWeapon->getBaseDamage();
        equippedWeapon->onHit(); 
        
        if (equippedWeapon->isBroken()) {
            std::cout << "\n*CRACK* " << name << "'s " << equippedWeapon->getName() << " shatters into pieces!\n";
            delete equippedWeapon;
            equippedWeapon = nullptr; 
        }
    }
    
    int finalDmg = calculateDamage(baseDmg);


    for (int i = 0; i < activeModifiers.size(); i++) {
        if (activeModifiers[i].getStatType() == DAMAGE) {
            finalDmg += activeModifiers[i].getValue();
        }
    }

    std::cout << name << " strikes " << target->getName() << " for " << finalDmg << " damage!\n";
    target->takeDamage(finalDmg);
}



void Player::useItem(int index) {
    if (index < 0) return; 
    if (inventory == nullptr) {
        std::cout << name << " reaches for an item, but has no inventory equipped!\n";
        return;
    }
    
    Item* itemToUse = inventory->getItem(index);
    if (!itemToUse) {
        std::cout << name << " grasps blindly at an empty inventory slot [" << index << "].\n";
        return;
    }


    Consumable* consumable = dynamic_cast<Consumable*>(itemToUse);
    if (consumable) {
        consumable->use(this); 
        inventory->removeItem(index); 
        delete consumable; 
        return;
    } 
    
    // SCENARIO B: It's a weapon/armor
    Equippable* equippable = dynamic_cast<Equippable*>(itemToUse);
    if (equippable) {
        inventory->removeItem(index); 
        equip(equippable); 
        return;
    }

    std::cout << name << " doesn't know how to use that right now.\n";
}

void Player::castSpell(int index, Entity* target) {
    if (index < 0) return; 
    
    if (spellbook == nullptr) {
        std::cout << name << " tries to cast a spell, but holds no spellbook!\n";
        return;
    }
    
    if (!target || target->isDead()) {
        std::cout << name << " attempts a spell, but there is no valid target!\n";
        return;
    }
    
    Spell* spell = spellbook->getSpell(index);
    if (!spell) {
        std::cout << name << " flips to a totally blank page trying to cast spell [" << index << "]!\n";
        return;
    }

    int totalCost = spell->getManaCost() + static_cast<int>(Spell::globalManaTax);

    if (mana >= totalCost) {
        mana -= totalCost;
        spell->cast(this, target);
        
        if (Spell::globalManaTax > 0) {
            std::cout << "(A global mana tax of " << Spell::globalManaTax << " was applied to the cast.)\n";
        }
    } else {
        std::cout << name << " tries to cast " << spell->getName() << " but lacks the required mana (" << totalCost << ")!\n";
    }
}

void Player::equip(Equippable* item) {
    if (item == nullptr) {
        std::cout << name << " fumbles and drops the item before equipping it.\n";
        return;
    }
    
    Weapon* wpn = dynamic_cast<Weapon*>(item);
    if (wpn) {
        // If we already have a weapon, put it back in the bag!
        if (equippedWeapon != nullptr) {
            equippedWeapon->onUnequip(this);
            std::cout << name << " unequips the " << equippedWeapon->getName() << ".\n";
            
            if (!inventory->addItem(equippedWeapon)) {
                std::cout << "Inventory full! The " << equippedWeapon->getName() << " falls to the ground and is lost.\n";
                delete equippedWeapon; 
            }
        }
        equippedWeapon = wpn;
        wpn->onEquip(this);
        std::cout << name << " equips the " << wpn->getName() << "!\n";
        return;
    }

    Armor* arm = dynamic_cast<Armor*>(item);
    if (arm) {
        // If we already have armor, put it back in the bag
        if (equippedArmor != nullptr) {
            equippedArmor->onUnequip(this);
            std::cout << name << " unequips the " << equippedArmor->getName() << ".\n";
            
            if (!inventory->addItem(equippedArmor)) {
                std::cout << "Inventory full! The " << equippedArmor->getName() << " falls to the ground and is lost.\n";
                delete equippedArmor;
            }
        }
        equippedArmor = arm;
        arm->onEquip(this);
        std::cout << name << " dons the " << arm->getName() << "!\n";
        return;
    }
    
    std::cout << name << " tries to securely equip an item structurally incompatible with their defensive frame.\n";
}

void Player::applyModifier(StatModifier mod) {
    if (activeModifiers.size() < modifierCapacity) {
        activeModifiers.push_back(mod);
        
        std::cout << "A new stat modifier attaches to " << name << ". Active: " << activeModifiers.size() << "/" << modifierCapacity << "\n";
    } else {
        std::cout << name << "'s system is overloaded and cannot accept more modifiers!\n";
    }
}

void Player::addSkill(Skill* skill) {
    if (skill != nullptr && unlockedSkills.size() < skillCapacity) {
        unlockedSkills.push_back(skill);
    }
}

void Player::clearModifiers() {
    activeModifiers.clear();
    std::cout << "All active stat modifiers have been aggressively purged from " << name << ".\n";
}

void Player::restoreMana(int amount) {
    if (amount <= 0) return;
    mana += amount;
    if (mana > 100) mana = 100; // Hard cap derived from standard initiation bounds
    std::cout << name << " channels energy, restoring " << amount << " mana! (Current: " << mana << "/100)\n";
}

void Player::takeTurn(Entity*)  {
    std::cout << "\n" << *this << " stands ready for their action sequence!\n";

    for (int i = 0; i < activeModifiers.size(); i++) {
        activeModifiers[i].decreaseDuration();
        
        if (activeModifiers[i].getDuration() <= 0) {
            std::cout << "A stat upgrade has worn off of " << name << ".\n";
            
            for (int j = i; j < activeModifiers.size() - 1; j++) {
                activeModifiers[j] = activeModifiers[j + 1];
            }
            i--; 
        }
    }
}
// OOP Operator Overload logic modifying explicit internal bounds natively
Player& Player::operator+(int healAmount) {
    std::cout << "Radiant light floods the entity. Healing for " << healAmount << " points!\n";
    takeDamage(-healAmount); // Utilizes bounds checked inside Entity
    return *this;
}


void Player::takeDamage(int amount) {
    int actualDamage = amount;

    if (amount > 0 && equippedArmor != nullptr) {
        actualDamage -= equippedArmor->getBaseDefense();
        
        equippedArmor->onHit();
        
        if (equippedArmor->isBroken()) {
            std::cout << "\n*SHATTER* " << name << "'s " << equippedArmor->getName() << " falls apart!\n";
            delete equippedArmor;
            equippedArmor = nullptr;
        }
    }

    
    if (actualDamage > 0) {
        for (int i = 0; i < activeModifiers.size(); i++) {
            if (activeModifiers[i].getStatType() == DEFENSE) {
                actualDamage -= activeModifiers[i].getValue();
            }
        }
    }

    if (actualDamage < 1 && amount > 0) actualDamage = 1; 

    Entity::takeDamage(actualDamage); 
}


void Player::serialize(json& j) const {
    Entity::serialize(j); 
    
    j["xp"] = xp;
    j["gold"] = gold;
    j["mana"] = mana;

    if (inventory != nullptr) {
        json invJson;
        inventory->serialize(invJson);
        j["inventory"] = invJson;
    }

    if (spellbook != nullptr) {
        json spellJson;
        spellbook->serialize(spellJson);
        j["spellbook"] = spellJson;
    }

    if (equippedWeapon != nullptr) {
        json weaponJson;
        equippedWeapon->serialize(weaponJson);
        j["equippedWeapon"] = weaponJson;
    }

    if (equippedArmor != nullptr) {
        json armorJson;
        equippedArmor->serialize(armorJson);
        j["equippedArmor"] = armorJson;
    }

    json modifierArray = json::array();
    for(int i = 0; i < activeModifiers.size(); i++) {
        modifierArray.push_back(activeModifiers[i]);
    }
    j["activeModifiers"] = modifierArray;

    json skillsArray = json::array();
    for (int i = 0; i < unlockedSkills.size(); i++) {
        json skill;
        unlockedSkills[i]->serialize(skill);
        skillsArray.push_back(skill); 
    }
    j["unlockedSkills"] = skillsArray;
}

void Player::deserialize(const json& j) {
    // 1. Load base Entity stats
    Entity::deserialize(j); 
    
    // 2. Load primitive Player stats
    xp = j.value("xp", 0);
    gold = j.value("gold", 0);
    mana = j.value("mana", 100);

    // 3. Reset combat modifiers safely
    activeModifiers.clear();
    json modifierArray = json::array();
    if(j.contains("activeModifier")) {
        for(const auto& _ : j["activeModifiers"]) {
            modifierArray.push_back(modifierArray.get<StatModifier>());
        }
    }

    // 4. Load the Inventory
    if (j.contains("inventory")) {
        if (inventory == nullptr) inventory = new Inventory(10); // Default capacity
        inventory->deserialize(j["inventory"]);
    }

    // 5. Load the Spellbook
    if (j.contains("spellbook")) {
        if (spellbook == nullptr) spellbook = new Spellbook();
        spellbook->deserialize(j["spellbook"]);
    }

    
    if (j.contains("equippedWeapon")) {
        if (equippedWeapon != nullptr) {
            delete equippedWeapon;
            equippedWeapon = nullptr;
        }
        
        std::string type = j["equippedWeapon"].value("type", "Unknown");
        
        if (type == "Sword") equippedWeapon = new Sword();
        else if (type == "Axe") equippedWeapon = new Axe();
        else if (type == "Dagger") equippedWeapon = new Dagger();

        if (equippedWeapon != nullptr) {
            equippedWeapon->deserialize(j["equippedWeapon"]);
        }
    }

    if (j.contains("equippedArmor")) {
        if (equippedArmor != nullptr) {
            delete equippedArmor;
            equippedArmor = nullptr;
        }
        
        std::string type = j["equippedArmor"].value("type", "Unknown");
        
        if (type == "ChainMail") equippedArmor = new ChainMail();
        else if (type == "PlateArmor") equippedArmor = new PlateArmor();

        if (equippedArmor != nullptr) {
            equippedArmor->deserialize(j["equippedArmor"]);
        }
    }

    unlockedSkills.clear(); // Reset the skill count
    
    if (j.contains("unlockedSkills")) {
        for (const auto& jsonSkill : j["unlockedSkills"]) {
            Skill* skill = nullptr;
            string type = j.value("type", "");
            if(type == "Passive") skill = new PassiveSkill();
            else if(type == "Active") skill = new ActiveSkill();
            else if(type == "Scaling") skill = new ScalingSkill();

            if(skill) {
                skill->deserialize(jsonSkill);
                addSkill(skill);
            }
            
        }
    }
}
