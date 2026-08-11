#ifndef SPELLBOOK_H
#define SPELLBOOK_H
#include "DynamicArray.h"
#include "Spell.h"
#include "Serializable.h"
class Spellbook : public Serializable {
private:
// COMPOSITION: The Spellbook *has a* TemplateArray.
// This replaces the double pointer (Spell**) from the UML.
    DynamicArray<Spell*> spells;

public:
    Spellbook();
    ~Spellbook();
// The TemplateArray handles all DMA, so no custom destructor needed here!
    void addSpell(Spell* s);
    void removeSpell(int index);
    void viewSpells() const;

    Spell* getSpell(int index) { if(index >= 0 && index < spells.size()) return spells[index]; else return nullptr; }
    void display() const;
    void serialize(json& j) const override;
    void deserialize(const json& j) override;
};
#endif