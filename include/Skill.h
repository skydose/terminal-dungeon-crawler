#ifndef SKILL_H
#define SKILL_H
#include <iostream>
#include <fstream>
#include <string>
#include "StatModifier.h"
#include "Serializable.h"
class Player;

// Skill is an abstract parent class
class Skill : public Serializable {
protected:
    std::string name;
    int requiredLevel;

public:
    Skill();
    Skill(std::string name, int requiredLevel);
    // Virtual destructor is needed as we use Skill* for child objects
    virtual ~Skill();
    std::string getName() const;
    int getRequiredLevel() const;

    void setName(std::string name);
    void setRequiredLevel(int requiredLevel);

    // virtual function,makes Skill abstract and supports runtime polymorphism.
    virtual void activate(Player* target) = 0;

    virtual std::string getType() const = 0;
    virtual void serialize(json& j) const override {
        j["name"] = name;
        j["requiredLevel"] = requiredLevel;
        j["type"] = "None";
    }
    virtual void deserialize(const json& j) {
        name = j.value("name", "");
        requiredLevel = j.value("requiredLevel", 0);
    }

    friend std::ostream& operator<<(std::ostream& out, const Skill& skill);
};

// ActiveSkill is a skill that the player uses directly.
class ActiveSkill : public Skill {
private:
    int staminaCost;
    StatModifier effect;

public:
    ActiveSkill();
    ActiveSkill(std::string name, int requiredLevel, int staminaCost, StatModifier effect);

    int getStaminaCost() const;
    StatModifier getEffect() const;

    void setStaminaCost(int staminaCost);
    void setEffect(StatModifier effect);
    // Overrides Skill's pure virtual function
    void activate(Player* target) override;

    std::string getType() const override;
    void serialize(json &j) const override;
    void deserialize(const json &j) override;
};
// Passive skill gives a basic stat modifier.
class PassiveSkill : public Skill {
private:
    StatModifier modifier;
public:
    PassiveSkill();
    PassiveSkill(std::string name, int requiredLevel, StatModifier modifier);

    StatModifier getModifier() const;
    void setModifier(StatModifier modifier);

    void activate(Player* target) override;
    std::string getType() const override;

    void serialize(json &j) const override;
    void deserialize(const json &j) override;

};
// Scaling skill becomes stronger with player level.
class ScalingSkill : public Skill {
private:
    float multiplier;
    StatModifier baseModifier;
public:
    ScalingSkill();
    ScalingSkill(std::string name, int requiredLevel, float multiplier, StatModifier baseModifier);

    float getMultiplier() const;
    StatModifier getBaseModifier() const;

    void setMultiplier(float multiplier);
    void setBaseModifier(StatModifier baseModifier);

    void activate(Player* target) override;
    std::string getType() const override;
    void serialize(json &j) const override;
    void deserialize(const json &j) override;
};
#endif