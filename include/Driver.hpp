#pragma once

#include "Position.hpp"
#include "Types.hpp"

class Driver {
public:
    Driver(
        int id,
        Position position
    );

    int getId() const;

    Position getPosition() const;
    void setPosition(Position position);

    DriverStatus getStatus() const;
    void setStatus(DriverStatus status);

private:
    int id;
    Position position;
    DriverStatus status;
};