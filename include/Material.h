#ifndef MATERIAL_H
#define MATERIAL_H

#include "Item.h"

enum class MaterialType
{
    WOOD,
    IRON,
    CRYSTAL
};

class Material : public Item
{
private:
    MaterialType type;

public:
    Material(const std::string &name, MaterialType type, int value);
    Material(const Material &other);
    std::string getType() const override;
    MaterialType getMaterialType() const;
    // std::string toJSON() const override;
};

#endif