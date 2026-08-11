#ifndef STATMODIFIER_H
#define STATMODIFIER_H
#include <iostream>
#include <fstream>
#include <string>
#include "Serializable.h"

enum StatType {
    NONE,
    DAMAGE,
    DEFENSE
};

class StatModifier {
public:
    StatType stat;
    int value;
    int duration;
    StatModifier();
    StatModifier(StatType stat, int value);
    StatModifier(StatType type, int val, int turns);


    std::string getStatName() const;
    StatType getStatType() const;
    int getValue() const;
    int getDuration() const;

    void setStatType(StatType type);
    void setValue(int val);
    void setDuration(int turns);

    void decreaseDuration();

    StatModifier operator+(const StatModifier& other) const;   // Operator overloading.
    // This combines two modifiers if both are for the same stat.

    friend void to_json(json& j, const StatModifier& modifier) {
        j["stat"] = modifier.stat;
        j["value"] = modifier.value;
        j["duration"] = modifier.duration;
    }

    friend void from_json(const json &j, StatModifier& modifier) {
        j.at("stat").get_to(modifier.stat);
        modifier.value = j.value("value", 0);
        modifier.duration = j.value("duration", 0); 
    }
    friend std::ostream& operator<<(std::ostream& out, const StatModifier& mod);
};

#endif 