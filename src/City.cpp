#include "City.hpp"

City::City(int id, const std::string& name, Position position)
    : id(id), name(name), position(position) {
}

int City::getId() const {
    return id;
}

const std::string& City::getName() const {
    return name;
}

Position City::getPosition() const {
    return position;
}