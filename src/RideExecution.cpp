#include "RideExecution.hpp"

#include <cmath>
#include <iostream>

#include "Geo.hpp"

void RideExecution::moveDriverTowards(
    Driver& driver,
    const Position& target
) {
    Position current = driver.getPosition();

    double dx = target.x - current.x;
    double dy = target.y - current.y;

    double distance = std::sqrt(
        dx * dx + dy * dy
    );

    // Driver has reached the target.
    if (distance <= DRIVER_SPEED) {
        driver.setPosition(target);
        return;
    }

    double ratio = DRIVER_SPEED / distance;

    Position newPosition{
        current.x + dx * ratio,
        current.y + dy * ratio
    };

    driver.setPosition(newPosition);
}

Rider* RideExecution::findRider(
    std::vector<Rider>& riders,
    int riderId
) {
    for (Rider& rider : riders) {
        if (rider.getId() == riderId) {
            return &rider;
        }
    }

    return nullptr;
}

void RideExecution::update(
    std::vector<Driver>& drivers,
    std::vector<Rider>& riders,
    std::vector<Ride>& rides
) {
    for (Ride& ride : rides) {

        // Only matched rides need execution.
        if (ride.getStatus() != RideStatus::MATCHED) {
            continue;
        }

        Rider* rider = findRider(
            riders,
            ride.getRiderId()
        );

        if (rider == nullptr) {
            continue;
        }

        Driver* driver = nullptr;

        for (Driver& candidate : drivers) {
            if (candidate.getId() == ride.getDriverId()) {
                driver = &candidate;
                break;
            }
        }

        if (driver == nullptr) {
            continue;
        }

        /*
         * PHASE 1:
         * Driver is travelling towards the rider's pickup location.
         */
        if (rider->getStatus() == RiderStatus::MATCHED) {

            double distance = euclideanDistance(
                driver->getPosition(),
                rider->getPickup()
            );

            if (distance <= DRIVER_SPEED) {

                driver->setPosition(
                    rider->getPickup()
                );

                rider->setStatus(
                    RiderStatus::IN_RIDE
                );

                std::cout
                    << "  Driver "
                    << driver->getId()
                    << " picked up Rider "
                    << rider->getId()
                    << '\n';

            } else {

                moveDriverTowards(
                    *driver,
                    rider->getPickup()
                );

                std::cout
                    << "  Driver "
                    << driver->getId()
                    << " moving to Rider "
                    << rider->getId()
                    << '\n';
            }

            continue;
        }

        /*
         * PHASE 2:
         * Rider is inside the vehicle.
         * Driver travels towards the destination.
         */
        if (rider->getStatus() == RiderStatus::IN_RIDE) {

            double distance = euclideanDistance(
                driver->getPosition(),
                rider->getDestination()
            );

            if (distance <= DRIVER_SPEED) {

                driver->setPosition(
                    rider->getDestination()
                );

                // Complete the ride.
                ride.setStatus(
                    RideStatus::COMPLETED
                );

                // Complete the rider's journey.
                rider->setStatus(
                    RiderStatus::COMPLETED
                );

                // Driver becomes available again.
                driver->setStatus(
                    DriverStatus::IDLE
                );

                std::cout
                    << "  Ride "
                    << ride.getId()
                    << " completed by Driver "
                    << driver->getId()
                    << '\n';

                std::cout
                    << "  Driver "
                    << driver->getId()
                    << " is now IDLE"
                    << '\n';

            } else {

                moveDriverTowards(
                    *driver,
                    rider->getDestination()
                );

                std::cout
                    << "  Driver "
                    << driver->getId()
                    << " driving Rider "
                    << rider->getId()
                    << " to destination"
                    << '\n';
            }
        }
    }
}