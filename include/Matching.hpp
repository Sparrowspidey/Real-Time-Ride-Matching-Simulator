#pragma once

#include <vector>

#include "Driver.hpp"
#include "Rider.hpp"
#include "Ride.hpp"

class MatchingEngine {
public:
    void matchRides(
        std::vector<Driver>& drivers,
        std::vector<Rider>& riders,
        std::vector<Ride>& rides
    );

private:
    int findNearestDriver(
        const std::vector<Driver>& drivers,
        const Rider& rider
    ) const;
};