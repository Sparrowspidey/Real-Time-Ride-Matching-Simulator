
#pragma once

#include <vector>

#include "Config.hpp"
#include "Driver.hpp"
#include "Rider.hpp"
#include "Ride.hpp"
#include "Matching.hpp"
#include "RideExecution.hpp"

enum class MatchingMode {
    SERIAL,
    PARALLEL
};

class Simulation {
public:
    explicit Simulation(
        const Config& config
    );

    void run();

    long long getTotalMatchedRides() const;
    double getTotalMatchingTimeMs() const;
    double getAverageMatchingTimeMs() const;

private:
    void initializeDrivers();
    void initializeRiders();
    void generateRideRequests();
    void tick();
    void saveBenchmarkResult() const;

    int numDrivers_;
    int numRiders_;
    int numTicks_;
    int currentTick_;

    double requestProbability_;

    MatchingMode matchingMode_;

    long long matchingOperations_;
    long long totalMatchedRides_;
    double totalMatchingTimeMs_;

    std::vector<Driver> drivers_;
    std::vector<Rider> riders_;
    std::vector<Ride> rides_;

    MatchingEngine matchingEngine_;
    RideExecution rideExecution_;
};

