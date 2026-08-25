#include <iostream>

#include "Driver.hpp"
#include "Rider.hpp"
#include "Geo.hpp"

int main() {

    Driver driver(
        1,
        {10.0, 20.0}
    );

    Rider rider(
        1,
        {13.0, 24.0},
        {50.0, 60.0},
        0
    );

    double distance = euclideanDistance(
        rider.getPickup(),
        driver.getPosition()
    );

    std::cout << "Driver ID: "
              << driver.getId()
              << '\n';

    std::cout << "Distance to rider: "
              << distance
              << '\n';

    return 0;
}