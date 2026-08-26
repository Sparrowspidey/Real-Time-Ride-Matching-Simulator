#pragma once

#include <string>
#include "Position.hpp"

class City {
private:
    int id;
    std::string name;
    Position position;

public:
    City(int id, const std::string& name, Position position);

    int getId() const;
    const std::string& getName() const;
    Position getPosition() const;
};