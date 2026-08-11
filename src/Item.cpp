#include "Item.h"
#include <sstream>

int Item::totalItemsCreated = 0;

Item::Item(const std::string &n, int v)
    : name(n), value(v)
{
    ++totalItemsCreated;
}

Item::Item(const Item &other)
    : name(other.name), value(other.value)
{
    ++totalItemsCreated;
}

Item::~Item()
{
    --totalItemsCreated;
}

std::string Item::getName() const { return name; }
int Item::getValue() const { return value; }
void Item::setValue(int v) { value = v; }

// std::string Item::toJSON() const
// {
//     std::ostringstream oss;
//     oss << "{\"type\":\"" << getType() << "\""
//         << ",\"name\":\"" << name << "\""
//         << ",\"value\":" << value << "}";
//     return oss.str();
// }

// bool Item::operator<(const Item &other) const { return value < other.value; }
// bool Item::operator>(const Item &other) const { return value > other.value; }

// std::ostream &operator<<(std::ostream &os, const Item &item)
// {
//     os << "[" << item.getType() << "] "
//        << item.name << " | " << item.value << "g";
//     return os;
// }