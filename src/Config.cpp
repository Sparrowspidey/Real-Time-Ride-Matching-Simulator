#include "Config.hpp"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

Config Config::load(const std::string& filename) {

    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error(
            "Could not open configuration file: " + filename
        );
    }

    json data;
    file >> data;

    Config config;

    config.drivers = data.at("drivers").get<int>();
    config.riders = data.at("riders").get<int>();
    config.ticks = data.at("ticks").get<int>();

    config.requestProbability =
        data.at("request_probability").get<double>();

    config.matchingMode =
        data.at("matching_mode").get<std::string>();

    config.threads =
        data.at("threads").get<int>();

    if (config.drivers <= 0) {
        throw std::runtime_error(
            "drivers must be greater than 0"
        );
    }

    if (config.riders <= 0) {
        throw std::runtime_error(
            "riders must be greater than 0"
        );
    }

    if (config.ticks <= 0) {
        throw std::runtime_error(
            "ticks must be greater than 0"
        );
    }

    if (config.requestProbability < 0.0 ||
        config.requestProbability > 1.0) {

        throw std::runtime_error(
            "request_probability must be between 0 and 1"
        );
    }

    if (config.threads <= 0) {
        throw std::runtime_error(
            "threads must be greater than 0"
        );
    }

    if (config.matchingMode != "serial" &&
        config.matchingMode != "parallel") {

        throw std::runtime_error(
            "matching_mode must be 'serial' or 'parallel'"
        );
    }

    return config;
}
