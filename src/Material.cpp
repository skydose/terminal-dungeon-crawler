// #include "Material.h"
// #include <sstream>

// static std::string matTypeToStr(MaterialType t)
// {
//     switch (t)
//     {
//     case MaterialType::WOOD:
//         return "WOOD";
//     case MaterialType::IRON:
//         return "IRON";
//     case MaterialType::CRYSTAL:
//         return "CRYSTAL";
//     default:
//         return "UNKNOWN";
//     }
// }

// Material::Material(const std::string &n, MaterialType t, int v)
//     : Item(n, v), type(t) {}

// Material::Material(const Material &other)
//     : Item(other), type(other.type) {}

// std::string Material::getType() const { return "Material"; }
// MaterialType Material::getMaterialType() const { return type; }

// // std::string Material::toJSON() const
// // {
// //     std::ostringstream oss;
// //     oss << "{\"type\":\"Material\""
// //         << ",\"name\":\"" << name << "\""
// //         << ",\"value\":" << value
// //         << ",\"materialType\":\"" << matTypeToStr(type) << "\"}";
// //     return oss.str();
// // }