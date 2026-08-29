#include <iostream>
#include <limits>
#include <exception>

#include "Config.hpp"
#include "Simulation.hpp"

namespace {

// Reads one line of input and converts it with `parse`.
// If the user just presses Enter, `defaultValue` is kept.
// If the input is invalid, the user is asked again.
template <typename T, typename ParseFn>
T promptValue(
    const std::string& label,
    T defaultValue,
    ParseFn parse
) {

    while (true) {

        std::cout
            << label
            << " [default: "
            << defaultValue
            << "]: ";

        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) {
            return defaultValue;
        }

        try {
            return parse(line);
        }
        catch (...) {
            std::cout
                << "  Invalid value, please try again.\n";
        }
    }
}

// Interactively asks the user whether to run serial or
// parallel, and lets them override drivers/riders/ticks/
// threads/request-probability. Pressing Enter on any
// question keeps the value loaded from config.json.
void promptForConfig(Config& config) {

    std::cout
        << "========================================\n";
    std::cout
        << " Simulation Setup\n";
    std::cout
        << "========================================\n";

    std::cout << "Select matching mode:\n";
    std::cout << "  1) Serial\n";
    std::cout << "  2) Parallel (OpenMP)\n";

    std::string modeDefault =
        (config.matchingMode == "serial") ? "1" : "2";

    std::string modeChoice = promptValue<std::string>(
        "Mode (1=Serial, 2=Parallel)",
        modeDefault,
        [](const std::string& s) {
            if (s != "1" && s != "2") {
                throw std::runtime_error("bad mode");
            }
            return s;
        }
    );

    config.matchingMode =
        (modeChoice == "1") ? "serial" : "parallel";

    config.drivers = promptValue<int>(
        "Number of drivers",
        config.drivers,
        [](const std::string& s) {
            int v = std::stoi(s);
            if (v <= 0) throw std::runtime_error("bad");
            return v;
        }
    );

    config.riders = promptValue<int>(
        "Number of riders",
        config.riders,
        [](const std::string& s) {
            int v = std::stoi(s);
            if (v <= 0) throw std::runtime_error("bad");
            return v;
        }
    );

    config.ticks = promptValue<int>(
        "Number of simulation ticks",
        config.ticks,
        [](const std::string& s) {
            int v = std::stoi(s);
            if (v <= 0) throw std::runtime_error("bad");
            return v;
        }
    );

    config.requestProbability = promptValue<double>(
        "Ride request probability (0.0 - 1.0)",
        config.requestProbability,
        [](const std::string& s) {
            double v = std::stod(s);
            if (v < 0.0 || v > 1.0) {
                throw std::runtime_error("bad");
            }
            return v;
        }
    );

    if (config.matchingMode == "parallel") {

        config.threads = promptValue<int>(
            "Number of OpenMP threads",
            config.threads,
            [](const std::string& s) {
                int v = std::stoi(s);
                if (v <= 0) throw std::runtime_error("bad");
                return v;
            }
        );

    } else {

        // Threads are irrelevant in serial mode, but keep
        // the value at 1 so it is reported correctly.
        config.threads = 1;
    }

    std::cout
        << "========================================\n\n";
}

}

int main() {

    try {

        Config config =
            Config::load("config/config.json");

        promptForConfig(config);

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