#pragma once

#include "Position.hpp"
#include "Types.hpp"

class Rider {
public:
    Rider(
        int id,
        Position pickup,
        Position destination,
        int requestTick
    );

    int getId() const;

    Position getPickup() const;
    Position getDestination() const;

    int getRequestTick() const;
    void setRequestTick(int tick);

    RiderStatus getStatus() const;
    void setStatus(RiderStatus status);

private:
    int id;

    Position pickup;
    Position destination;

    int requestTick;

    RiderStatus status;
};