#pragma once

class Simulation {
public:
    Simulation(int numDrivers, int numRiders, int numTicks);

    void run();

private:
    void tick();

    int numDrivers_;
    int numRiders_;
    int numTicks_;
    int currentTick_;
};