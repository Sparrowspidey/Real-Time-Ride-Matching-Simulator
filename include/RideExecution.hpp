#pragma once

#include <vector>

#include "Driver.hpp"
#include "Rider.hpp"
#include "Ride.hpp"

class RideExecution {
public:
    void update(
        std::vector<Driver>& drivers,
        std::vector<Rider>& riders,
        std::vector<Ride>& rides
    );

private:
    static constexpr double DRIVER_SPEED = 10.0;

    void moveDriverTowards(
        Driver& driver,
        const Position& target
    );

    Rider* findRider(
        std::vector<Rider>& riders,
        int riderId
    );
};