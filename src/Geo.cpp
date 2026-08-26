#include "Geo.hpp"

#include <cmath>

double euclideanDistance(
    const Position& a,
    const Position& b
) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;

    return std::sqrt(
        dx * dx + dy * dy
    );
}