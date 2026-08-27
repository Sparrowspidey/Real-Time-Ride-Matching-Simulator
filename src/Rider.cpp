#include "Rider.hpp"

Rider::Rider(
    int id,
    Position pickup,
    Position destination,
    int requestTick
)
    : id(id),
      pickup(pickup),
      destination(destination),
      requestTick(requestTick),
      status(RiderStatus::IDLE) {
}

int Rider::getId() const {
    return id;
}

Position Rider::getPickup() const {
    return pickup;
}

Position Rider::getDestination() const {
    return destination;
}

int Rider::getRequestTick() const {
    return requestTick;
}

void Rider::setRequestTick(int tick) {
    requestTick = tick;
}

RiderStatus Rider::getStatus() const {
    return status;
}

void Rider::setStatus(RiderStatus status) {
    this->status = status;
}