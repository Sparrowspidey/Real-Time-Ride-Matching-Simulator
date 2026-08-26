#include "Ride.hpp"

Ride::Ride(int id, int pickupCityId, int destinationCityId, int requestTime)
    : id(id),
      pickupCityId(pickupCityId),
      destinationCityId(destinationCityId),
      requestTime(requestTime),
      status(RideStatus::REQUESTED) {
}

int Ride::getId() const {
    return id;
}

int Ride::getPickupCityId() const {
    return pickupCityId;
}

int Ride::getDestinationCityId() const {
    return destinationCityId;
}

int Ride::getRequestTime() const {
    return requestTime;
}

RideStatus Ride::getStatus() const {
    return status;
}

void Ride::setStatus(RideStatus status) {
    this->status = status;
}