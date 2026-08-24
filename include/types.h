#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>

typedef enum {
    DRIVER_IDLE = 0,
    DRIVER_BUSY = 1
} DriverStatus;

typedef struct {
    int id;
    double x;
    double y;
    DriverStatus status;
    int busy_until_tick;
} Driver;

typedef struct {
    int id;
    double x;
    double y;
    int request_tick;
    int matched_driver_id;
    int matched_tick;
    int wait_ticks;
} Rider;

typedef struct {
    int grid_size;
    int num_drivers;
    int num_riders;
    int ticks;
    unsigned int seed;
    int max_trip_ticks;
    double request_probability;
} Config;

#endif
