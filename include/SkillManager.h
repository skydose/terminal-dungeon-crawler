#ifndef SKILLMANAGER_H
#define SKILLMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include "Serializable.h"
#include "Skill.h"
#include "SkillRules.h"
#include "Player.h"


// Singleton class for managing all skills.
class SkillManager : public Serializable {
private:
    Skill** skills;
    int numSkills;
    int capacity;

    static SkillManager* instance;

    SkillManager();
    // Copying, blocked as this is singleton.
    SkillManager(const SkillManager&) = delete;
    SkillManager& operator=(const SkillManager&) = delete;

    

public:
    void resize();
    void clearSkills();
    ~SkillManager();
    static SkillManager* getInstance();
    static void destroyInstance();

    int getNumSkills() const;
    int getCapacity() const;

    bool learnSkill(Skill* skill);
    bool unlock(Player* player, int id);

    Skill* getSkill(int id) const;
    void showSkills() const;

    void saveToJson(const std::string& fileName) const;
    void loadFromJson(const std::string& fileName);

    void serialize(json& j) const override;
    void deserialize(const json& j) override;
};
#endif