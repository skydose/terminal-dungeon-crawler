#include "Skill.h"
#include "Player.h"
#include "StatModifier.h"


static std::string escapeJsonText(const std::string& text) {
    std::string result = "";
    for (size_t i = 0; i < text.length(); i++) {        // i changed int to size_t (which is a special unsigned int) because text.length() usually returns size_t and not int
        if (text[i] == '"') {
            result += "\\\"";
        }
        else if (text[i] == '\\') {
            result += "\\\\";
        }
        else {
            result += text[i];
        }
    }
    return result;
}

static void applyModifierToPlayer(Player* target, StatModifier modifier) {
    if (target == nullptr) {
        return;
    }

    target->applyModifier(modifier);
}




Skill::Skill() {
    name = "Unknown Skill";
    requiredLevel = 1;
}
Skill::Skill(std::string name, int requiredLevel) {
    this->name = name;
    if (requiredLevel < 1) {
        this->requiredLevel = 1;
    }
    else {
        this->requiredLevel = requiredLevel;
    }
}
Skill::~Skill() {
    // Empty as Skill has no direct dynamic memory
}

std::string Skill::getName() const {
    return name;
}

int Skill::getRequiredLevel() const {
    return requiredLevel;
}

void Skill::setName(std::string name) {
    this->name = name;
}

void Skill::setRequiredLevel(int requiredLevel) {
    if (requiredLevel < 1) {
        this->requiredLevel = 1;
    }
    else{
        this->requiredLevel = requiredLevel;
    }
}



std::ostream& operator<<(std::ostream& out, const Skill& skill) {
    out << skill.getType() << " Skill: " << skill.name
       << " | Required Level: " << skill.requiredLevel;
    return out;
}


ActiveSkill::ActiveSkill() : Skill() {
    staminaCost = 0;
    effect = StatModifier(DAMAGE, 0);
}

ActiveSkill::ActiveSkill(std::string name, int requiredLevel, int staminaCost, StatModifier effect)
: Skill(name, requiredLevel) {


    if (staminaCost < 0) {
        this->staminaCost = 0;
}

    else{
        this->staminaCost = staminaCost;
    }
    this->effect = effect;
}

int ActiveSkill::getStaminaCost() const {
    return staminaCost;
}

StatModifier ActiveSkill::getEffect() const {
    return effect;
}

void ActiveSkill::setStaminaCost(int staminaCost) {
    if (staminaCost < 0) {
        this->staminaCost = 0;
    }
    else {
        this->staminaCost = staminaCost;
    }
}

void ActiveSkill::setEffect(StatModifier effect) {
    this->effect = effect;
}

void ActiveSkill::activate(Player* target) {
    if (target == nullptr) {
        std::cout << "Active skill failed because player does not exist.\n";
        return;
    }

    applyModifierToPlayer(target, effect);
    std::cout << name << " activated. Stamina Cost: " << staminaCost
              << ". Effect: " << effect << "\n";
}

std::string ActiveSkill::getType() const {
    return "Active";
}


void ActiveSkill::serialize(json &j) const  {
    Skill::serialize(j);
    j["staminaCost"] = staminaCost;
    j["effect"] = effect;
    j["type"] = "Active";
}
void ActiveSkill::deserialize(const json &j)  {
    Skill::deserialize(j);
    staminaCost = j.value("staminaCost", 0);
    j.at("effect").get_to(effect);
}


PassiveSkill::PassiveSkill() : Skill() {
    modifier = StatModifier(DEFENSE, 0);
}

PassiveSkill::PassiveSkill(std::string name, int requiredLevel, StatModifier modifier)
: Skill(name, requiredLevel) {
    this->modifier = modifier;
}

StatModifier PassiveSkill::getModifier() const {
    return modifier;
}

void PassiveSkill::setModifier(StatModifier modifier) {
    this->modifier = modifier;
}

void PassiveSkill::activate(Player* target) {
    if (target == nullptr) {
        std::cout << "Passive skill failed because player does not exist.\n";
        return;
    }

    applyModifierToPlayer(target, modifier);
    std::cout << name << " passive skill applied. Modifier: "
              << modifier << "\n";
}

std::string PassiveSkill::getType() const {
    return "Passive";
}

void PassiveSkill::serialize(json &j) const {
    Skill::serialize(j);
    j["modifier"] = modifier;
    j["type"] = "Passive";
}
void PassiveSkill::deserialize(const json &j) {
    Skill::deserialize(j);
    j.at("modifier").get_to(modifier);
}


ScalingSkill::ScalingSkill() : Skill() {
    multiplier = 1.0f;
    baseModifier = StatModifier(DAMAGE, 1);
}
ScalingSkill::ScalingSkill(std::string name, int requiredLevel, float multiplier, StatModifier baseModifier)
: Skill(name, requiredLevel) {
    if (multiplier < 0.0f) {
        this->multiplier = 1.0f;
    }
    else {
        this->multiplier = multiplier;
    }
    this->baseModifier = baseModifier;
}

float ScalingSkill::getMultiplier() const {
    return multiplier;
}

StatModifier ScalingSkill::getBaseModifier() const {
    return baseModifier;
}

void ScalingSkill::setMultiplier(float multiplier) {
    if (multiplier < 0.0f) {
        this->multiplier = 1.0f;
    }
    else {
        this->multiplier = multiplier;
    }
}

void ScalingSkill::setBaseModifier(StatModifier baseModifier) {
    this->baseModifier = baseModifier;
}

void ScalingSkill::activate(Player* target) {
    if (target == nullptr) {
        std::cout << "Scaling skill failed because player does not exist.\n";
        return;
    }

    // Formula: base value * multiplier * player level.
    int scaledValue = static_cast<int>(baseModifier.value * multiplier * target->getLevel());
    if (scaledValue < 0) {
        scaledValue = 0;
    }

    StatModifier scaledModifier(baseModifier.stat, scaledValue);
    applyModifierToPlayer(target, scaledModifier);
    std::cout << name << " activated. Scaled effect: "
                << scaledModifier << "\n";
}

std::string ScalingSkill::getType() const {
    return "Scaling";
}

void ScalingSkill::serialize(json &j) const {
    Skill::serialize(j);
    j["multiplier"] = multiplier;
    j["baseModifier"] = baseModifier;
    j["type"] = "Scaling";
}
void ScalingSkill::deserialize(const json &j) {
    Skill::deserialize(j);
    multiplier = j.value("multiplier", 0);
    j.at("baseModifier").get_to(baseModifier);
}

