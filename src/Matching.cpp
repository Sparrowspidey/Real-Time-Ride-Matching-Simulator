#include "Matching.hpp"

#include <limits>
#include <iostream>

#include "Geo.hpp"

int MatchingEngine::findNearestDriver(
    const std::vector<Driver>& drivers,
    const Rider& rider
) const {

    int nearestDriverIndex = -1;
    double minimumDistance = std::numeric_limits<double>::max();

    for (int i = 0; i < static_cast<int>(drivers.size()); ++i) {

        // Only idle drivers can be matched.
        if (drivers[i].getStatus() != DriverStatus::IDLE) {
            continue;
        }

        double distance = euclideanDistance(
            drivers[i].getPosition(),
            rider.getPickup()
        );

        if (distance < minimumDistance) {
            minimumDistance = distance;
            nearestDriverIndex = i;
        }
    }

    return nearestDriverIndex;
}

void MatchingEngine::matchRides(
    std::vector<Driver>& drivers,
    std::vector<Rider>& riders,
    std::vector<Ride>& rides
) {

    for (Ride& ride : rides) {

        // Only requested rides need matching.
        if (ride.getStatus() != RideStatus::REQUESTED) {
            continue;
        }

        for (Rider& rider : riders) {

            if (rider.getId() != ride.getRiderId()) {
                continue;
            }

            int driverIndex = findNearestDriver(
                drivers,
                rider
            );

            if (driverIndex == -1) {
                std::cout
                    << "  No available driver for Ride "
                    << ride.getId()
                    << '\n';

                break;
            }

            Driver& driver = drivers[driverIndex];

            double distance = euclideanDistance(
                driver.getPosition(),
                rider.getPickup()
            );

            driver.setStatus(DriverStatus::BUSY);
            rider.setStatus(RiderStatus::MATCHED);

            ride.setDriverId(driver.getId());
            ride.setStatus(RideStatus::MATCHED);

            std::cout
                << "  Ride "
                << ride.getId()
                << " matched: Driver "
                << driver.getId()
                << " -> Rider "
                << rider.getId()
                << " (distance: "
                << distance
                << ")\n";

            break;
        }
    }
}