#include "Simulation.hpp"

#include <iostream>

Simulation::Simulation(
    int numDrivers,
    int numRiders,
    int numTicks
)
    : numDrivers_(numDrivers),
      numRiders_(numRiders),
      numTicks_(numTicks),
      currentTick_(0) {
}

void Simulation::run() {

    std::cout << "Starting simulation...\n";

    for (currentTick_ = 0;
         currentTick_ < numTicks_;
         ++currentTick_) {

        tick();
    }

    std::cout << "Simulation completed.\n";
}

void Simulation::tick() {

    std::cout << "Tick: "
              << currentTick_
              << '\n';
}