#pragma once

#include <string>

struct Config {
    int drivers;
    int riders;
    int ticks;

    double requestProbability;

    std::string matchingMode;

    int threads;

    static Config load(const std::string& filename);
};
