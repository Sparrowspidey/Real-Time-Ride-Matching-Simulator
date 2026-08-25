cmake_minimum_required(VERSION 3.16)

project(RealTimeRideMatchingSimulator
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# OpenMP
find_package(OpenMP REQUIRED)

add_executable(ride_sim
    src/main.cpp
    src/Driver.cpp
    src/Rider.cpp
    src/Geo.cpp
)

target_include_directories(ride_sim
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(ride_sim
    PRIVATE
        OpenMP::OpenMP_CXX
)