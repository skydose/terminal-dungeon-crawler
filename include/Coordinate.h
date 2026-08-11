#ifndef COORDINATE_H
#define COORDINATE_H

struct Coordinate {
    int x;
    int y;


    bool operator==(const Coordinate& other) const {
        return (this->x == other.x && this->y == other.y);
    }


    bool operator!=(const Coordinate& other) const {
        return !(*this == other);
    }


    Coordinate operator+(const Coordinate& other) const {
        return {this->x + other.x, this->y + other.y};
    }


    friend std::ostream& operator<<(std::ostream& os, const Coordinate& coord) {
        os << "[" << coord.x << ", " << coord.y << "]";
        return os;
    }
};

#endif