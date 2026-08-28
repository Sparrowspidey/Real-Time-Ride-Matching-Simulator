#include <iostream>
#include <exception>

#include "Config.hpp"
#include "Simulation.hpp"

int main() {

    try {

        Config config =
            Config::load("config/config.json");

        std::cout
            << "========================================\n";

        std::cout
            << " Real-Time Ride Matching Simulator\n";

        std::cout
            << "========================================\n";

        std::cout
            << "Drivers : "
            << config.drivers
            << '\n';

        std::cout
            << "Riders  : "
            << config.riders
            << '\n';

        std::cout
            << "Ticks   : "
            << config.ticks
            << '\n';

        std::cout
            << "Mode    : "
            << (config.matchingMode == "serial"
                ? "SERIAL"
                : "OPENMP PARALLEL")
            << '\n';

        std::cout
            << "Threads : "
            << config.threads
            << '\n';

        std::cout
            << "Request probability : "
            << config.requestProbability
            << '\n';

        std::cout
            << "========================================\n\n";

        Simulation simulation(config);

        simulation.run();

        std::cout
            << "\n========== BENCHMARK RESULT ==========\n";

        std::cout
            << "Matched rides       : "
            << simulation.getTotalMatchedRides()
            << '\n';

        std::cout
            << "Total matching time : "
            << simulation.getTotalMatchingTimeMs()
            << " ms\n";

        std::cout
            << "Average match/tick  : "
            << simulation.getAverageMatchingTimeMs()
            << " ms\n";

        std::cout
            << "======================================\n";

    }
    catch (const std::exception& exception) {

        std::cerr
            << "ERROR: "
            << exception.what()
            << '\n';

        return 1;
    }

    return 0;
}
