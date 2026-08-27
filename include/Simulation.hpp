#pragma once

#include <vector>

#include "Driver.hpp"
#include "Rider.hpp"
#include "Ride.hpp"
#include "Matching.hpp"
#include "RideExecution.hpp"

class Simulation {
public:
    Simulation(int numDrivers, int numRiders, int numTicks);

    void run();

private:
    void initializeDrivers();
    void initializeRiders();
    void generateRideRequests();
    void tick();

    int numDrivers_;
    int numRiders_;
    int numTicks_;
    int currentTick_;

    std::vector<Driver> drivers_;
    std::vector<Rider> riders_;
    std::vector<Ride> rides_;

    MatchingEngine matchingEngine_;
    RideExecution rideExecution_;
};