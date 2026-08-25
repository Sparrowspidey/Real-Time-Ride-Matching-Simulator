#include "Driver.hpp"

Driver::Driver(int id, Position position)
    : id(id),
      position(position),
      status(DriverStatus::IDLE) {
}

int Driver::getId() const {
    return id;
}

Position Driver::getPosition() const {
    return position;
}

void Driver::setPosition(Position position) {
    this->position = position;
}

DriverStatus Driver::getStatus() const {
    return status;
}

void Driver::setStatus(DriverStatus status) {
    this->status = status;
}