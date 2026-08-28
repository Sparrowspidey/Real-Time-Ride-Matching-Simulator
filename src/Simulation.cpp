
#include "Simulation.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <omp.h>
#include <random>
#include <chrono>
#include <filesystem>

namespace {

constexpr double CITY_WIDTH = 100.0;
constexpr double CITY_HEIGHT = 100.0;



}


Simulation::Simulation(
    const Config& config
)
    : numDrivers_(config.drivers),
      numRiders_(config.riders),
      numTicks_(config.ticks),
      currentTick_(0),
      requestProbability_(config.requestProbability),
      matchingMode_(
          config.matchingMode == "serial"
              ? MatchingMode::SERIAL
              : MatchingMode::PARALLEL
      ),
      matchingOperations_(0),
      totalMatchedRides_(0),
      totalMatchingTimeMs_(0.0) {

    omp_set_num_threads(config.threads);

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

        if (probability(generator) < requestProbability_) {

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

    std::cout << "\nTotal rides matched: "
              << totalMatchedRides_
              << '\n';

    std::cout << "Total matching time: "
              << totalMatchingTimeMs_
              << " ms\n";

    if (matchingOperations_ > 0) {

        std::cout << "Average matching time per tick: "
                  << totalMatchingTimeMs_ / matchingOperations_
                  << " ms\n";
    }

    std::cout << "Simulation completed.\n";

    saveBenchmarkResult();
}

void Simulation::tick() {

    std::cout << "Tick: "
              << currentTick_
              << '\n';

    generateRideRequests();

    auto matchingStart =
        std::chrono::high_resolution_clock::now();

    int matchedRides = 0;

    if (matchingMode_ == MatchingMode::SERIAL) {

        matchedRides = matchingEngine_.matchRidesSerial(
            drivers_,
            riders_,
            rides_
        );

    } else {

        matchedRides = matchingEngine_.matchRidesParallel(
            drivers_,
            riders_,
            rides_
        );
    }

    totalMatchedRides_ += matchedRides;

    auto matchingEnd =
        std::chrono::high_resolution_clock::now();

    double matchingTimeMs =
        std::chrono::duration<double, std::milli>(
            matchingEnd - matchingStart
        ).count();

    totalMatchingTimeMs_ += matchingTimeMs;

    ++matchingOperations_;

    rideExecution_.update(
        drivers_,
        riders_,
        rides_
    );
}

void Simulation::saveBenchmarkResult() const {

    const std::string resultsDirectory = "results";
    const std::string outputFile =
        resultsDirectory + "/benchmark.csv";

    std::filesystem::create_directories(
        resultsDirectory
    );

    std::ifstream existingFile(outputFile);

    bool fileExists = existingFile.good();

    existingFile.close();

    std::ofstream file(
        outputFile,
        std::ios::app
    );

    if (!file.is_open()) {

        std::cerr
            << "Warning: Could not open "
            << outputFile
            << " for writing.\n";

        return;
    }

    if (!fileExists) {

        file
            << "drivers,"
            << "riders,"
            << "ticks,"
            << "mode,"
            << "threads,"
            << "matched_rides,"
            << "total_matching_time_ms,"
            << "average_matching_time_ms\n";
    }

    int threadCount = 1;

    if (matchingMode_ == MatchingMode::PARALLEL) {
        threadCount = omp_get_max_threads();
    }

    file << numDrivers_ << ','
         << numRiders_ << ','
         << numTicks_ << ','
         << (matchingMode_ == MatchingMode::SERIAL
             ? "serial"
             : "parallel")
         << ','
         << threadCount
         << ','
         << totalMatchedRides_
         << ','
         << std::fixed
         << std::setprecision(6)
         << totalMatchingTimeMs_
         << ','
         << getAverageMatchingTimeMs()
         << '\n';

    file.close();

    std::cout
        << "Benchmark result saved to "
        << outputFile
        << '\n';
}

long long Simulation::getTotalMatchedRides() const {
    return totalMatchedRides_;
}

double Simulation::getTotalMatchingTimeMs() const {
    return totalMatchingTimeMs_;
}

double Simulation::getAverageMatchingTimeMs() const {

    if (matchingOperations_ == 0) {
        return 0.0;
    }

    return totalMatchingTimeMs_ /
           static_cast<double>(matchingOperations_);
}

