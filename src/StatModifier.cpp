#include "StatModifier.h"

StatModifier::StatModifier() {
    stat = NONE;
    value = 0;
    duration = 0;
}

StatModifier::StatModifier(StatType stat, int value) {
    this->stat = stat;
    this->value = value;
    duration = 0;
}

StatModifier::StatModifier(StatType type, int val, int turns) {
    stat = type;
    value = val;
    duration = turns;
}

StatType StatModifier::getStatType() const {
    return stat;
}

int StatModifier::getValue() const {
    return value;
}

int StatModifier::getDuration() const {
    return duration;
}

void StatModifier::setStatType(StatType type) {
    stat = type;
}

void StatModifier::setValue(int val) {
    value = val;
}

void StatModifier::setDuration(int turns) {
    duration = turns;
}

void StatModifier::decreaseDuration() {
    if (duration > 0) {
        duration--;
    }
}

std::string StatModifier::getStatName() const {
    if (stat == DAMAGE) {
        return "DAMAGE";
    }
    else if (stat == DEFENSE) {
        return "DEFENSE";
    }
    return "NONE";
}

StatModifier StatModifier::operator+(const StatModifier& other) const {
    // So,DAMAGE +5 and DAMAGE +10 becomes DAMAGE +15.
    if (this->stat == other.stat) {
        return StatModifier(this->stat, this->value + other.value);
    }
    // If stats are different, do not mix
    return *this;
}

std::ostream& operator<<(std::ostream& out, const StatModifier& mod) {
    out << mod.getStatName() << " +" << mod.value;
    return out;
}