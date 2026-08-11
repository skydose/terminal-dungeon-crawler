#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H
#include <fstream>
#include "json.hpp" 

using json = nlohmann::json;

class Serializable {
public:
    virtual void serialize(json& j) const = 0;
    virtual void deserialize(const json &j) = 0;
    virtual ~Serializable() {}
};

#endif