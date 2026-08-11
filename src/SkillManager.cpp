#include "SkillManager.h"
#include "Player.h"
#include "json.hpp"

using json = nlohmann::json;
SkillManager* SkillManager::instance = nullptr;

SkillManager::SkillManager() {
    capacity = SkillRules::defaultSkillCapacity;
    numSkills = 0;

    skills = new Skill*[capacity];

    for (int i = 0; i < capacity; i++) {
        skills[i] = nullptr;
    }
}



SkillManager::~SkillManager() {
    clearSkills();
    delete[] skills;
}

SkillManager* SkillManager::getInstance() {
    if (instance == nullptr) {
        instance = new SkillManager();
    }

    return instance;
}

void SkillManager::destroyInstance() {
    delete instance;
    instance = nullptr;
}

int SkillManager::getNumSkills() const {
    return numSkills;
}

int SkillManager::getCapacity() const {
    return capacity;
}

void SkillManager::clearSkills() {
    // As SkillManager owns the Skill objects, so it deletes them
    for (int i = 0; i < numSkills; i++) {
        delete skills[i];
        skills[i] = nullptr;
    }
    numSkills = 0;
}

void SkillManager::resize() {
    int newCapacity = capacity * 2;

    Skill** newSkills = new Skill*[newCapacity];
    for (int i = 0; i < newCapacity; i++) {
        newSkills[i] = nullptr;
    }

    for (int i = 0; i < numSkills; i++) {
        newSkills[i] = skills[i];
    }

    delete[] skills;
    skills = newSkills;
    capacity = newCapacity;
}

bool SkillManager::learnSkill(Skill* skill) {
    if (skill == nullptr) {
        std::cout << "Cannot learn an empty skill.\n";
        return false;
    }

    if (numSkills >= SkillRules::maxUnlockedSkills) {
        std::cout << "Maximum skill limit reached.\n";
        delete skill;
        return false;
    }

    if (numSkills >= capacity) {
        resize();
    }

    skills[numSkills] = skill;
    numSkills++;

    return true;
}

Skill* SkillManager::getSkill(int id) const {
    if (id < 0 || id >= numSkills) {
        return nullptr;
    }

    return skills[id];
}

bool SkillManager::unlock(Player* player, int id) {
    if (player == nullptr) {
        std::cout << "No player found.\n";
        return false;
    }

    if (id < 0 || id >= numSkills) {
        std::cout << "Invalid skill number.\n";
        return false;
    }

    Skill* selectedSkill = skills[id];
    if (selectedSkill == nullptr) {
        std::cout << "This skill slot is empty.\n";
        return false;
    }

    if (player->getLevel() < selectedSkill->getRequiredLevel()) {
        std::cout << "Player level is too low for this skill.\n";
        return false;
    }
    selectedSkill->activate(player);

    return true;
}

void SkillManager::showSkills() const {
    if (numSkills == 0) {
        std::cout << "No skills available.\n";
        return;
    }

    std::cout << "\n Available Skills \n";

    for (int i = 0; i < numSkills; i++) {
        if (skills[i] != nullptr) {
            std::cout << i << ". " << *skills[i] << "\n";
        }
    }
}

void SkillManager::saveToJson(const std::string& fileName) const {
    std::ofstream out(fileName);
    if (!out) {
        std::cout << "Could not create skill JSON file.\n";
        return;
    }
    json j;
    serialize(j);

    out << j.dump(4);
    out.close();
    std::cout << "Skill data saved to " << fileName << "\n";
}

void SkillManager::loadFromJson(const std::string& fileName) {
    std::ifstream in(fileName);
    if (!in) {
        std::cout << "Could not open skill JSON file.\n";
        return;
    }
    json j;
    in >> j;

    deserialize(j);
    in.close();
    std::cout << "Skill data loaded from " << fileName << "\n";
}


void SkillManager::serialize(json& j) const {

    j["module"] = "SkillManager";
    j["totalSkills"] = numSkills;
    j["capacity"] = capacity;
    j["skills"] = json::array();

    for (int i = 0; i < numSkills; i++) {
        if (skills[i] != nullptr) {
            json skillJson;

            skillJson["id"] = i;
            skillJson["name"] = skills[i]->getName();
            skillJson["type"] = skills[i]->getType();
            skillJson["requiredLevel"] = skills[i]->getRequiredLevel();

            j["skills"].push_back(skillJson);
        }
    }
}

void SkillManager::deserialize(const json& j) {
    std::cout << "\nLoaded Skill JSON\n";
    std::cout << j.dump(4) << "\n";
}