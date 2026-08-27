#include "Simulation.hpp"

#include <iostream>
#include <random>

namespace {

constexpr double CITY_WIDTH = 100.0;
constexpr double CITY_HEIGHT = 100.0;

// Probability that a rider creates a request on a tick.
constexpr double REQUEST_PROBABILITY = 0.10;

}

Simulation::Simulation(
    int numDrivers,
    int numRiders,
    int numTicks
)
    : numDrivers_(numDrivers),
      numRiders_(numRiders),
      numTicks_(numTicks),
      currentTick_(0) {

    initializeDrivers();
    initializeRiders();
}

void Simulation::initializeDrivers() {

    std::mt19937 generator(42);

    std::uniform_real_distribution<double> xDistribution(
        0.0,
        CITY_WIDTH
    );

    std::uniform_real_distribution<double> yDistribution(
        0.0,
        CITY_HEIGHT
    );

    drivers_.reserve(numDrivers_);

    for (int i = 0; i < numDrivers_; ++i) {

        Position position{
            xDistribution(generator),
            yDistribution(generator)
        };

        drivers_.emplace_back(
            i + 1,
            position
        );
    }
}

void Simulation::initializeRiders() {

    std::mt19937 generator(123);

    std::uniform_real_distribution<double> xDistribution(
        0.0,
        CITY_WIDTH
    );

    std::uniform_real_distribution<double> yDistribution(
        0.0,
        CITY_HEIGHT
    );

    riders_.reserve(numRiders_);

    for (int i = 0; i < numRiders_; ++i) {

        Position pickup{
            xDistribution(generator),
            yDistribution(generator)
        };

        Position destination{
            xDistribution(generator),
            yDistribution(generator)
        };

        riders_.emplace_back(
            i + 1,
            pickup,
            destination,
            0
        );
    }
}

void Simulation::generateRideRequests() {

    static std::mt19937 generator(456);

    std::uniform_real_distribution<double> probability(
        0.0,
        1.0
    );

    for (Rider& rider : riders_) {

        // Only idle riders can request a new ride.
        if (rider.getStatus() != RiderStatus::IDLE) {
            continue;
        }

        if (probability(generator) < REQUEST_PROBABILITY) {

            int rideId = static_cast<int>(rides_.size()) + 1;

            Ride ride(
                rideId,
                rider.getId(),
                0,
                0,
                currentTick_
            );

            rides_.push_back(ride);

            rider.setStatus(RiderStatus::PENDING);
            rider.setRequestTick(currentTick_);

            std::cout << "  New ride request: Ride "
                      << rideId
                      << " from Rider "
                      << rider.getId()
                      << '\n';
        }
    }
}

void Simulation::run() {

    std::cout << "Starting simulation...\n";

    std::cout << "Drivers initialized: "
              << drivers_.size()
              << '\n';

    std::cout << "Riders initialized: "
              << riders_.size()
              << '\n';

    std::cout << '\n';

    for (currentTick_ = 0;
         currentTick_ < numTicks_;
         ++currentTick_) {

        tick();
    }

    std::cout << "\nTotal ride requests: "
              << rides_.size()
              << '\n';

    std::cout << "Simulation completed.\n";
}

void Simulation::tick() {

    std::cout << "Tick: "
              << currentTick_
              << '\n';

    generateRideRequests();

    matchingEngine_.matchRides(
    drivers_,
    riders_,
    rides_
);

    rideExecution_.update(
    drivers_,
    riders_,
    rides_
    );
}
