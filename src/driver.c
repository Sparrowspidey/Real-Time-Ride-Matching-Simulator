#include "driver.h"
#include "utils.h"

void drivers_init(Driver *drivers, int n, int grid_size, unsigned int seed) {
    unsigned int s = seed;
    for (int i = 0; i < n; ++i) {
        drivers[i].id = i;
        drivers[i].x = (double)rng_int(&s, grid_size);
        drivers[i].y = (double)rng_int(&s, grid_size);
        drivers[i].status = DRIVER_IDLE;
        drivers[i].busy_until_tick = 0;
    }
}

void drivers_update(Driver *drivers, int n, int grid_size, int tick, unsigned int *seed) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; ++i) {
        if (drivers[i].status == DRIVER_BUSY && tick >= drivers[i].busy_until_tick) {
            drivers[i].status = DRIVER_IDLE;
        }

        if (drivers[i].status == DRIVER_IDLE) {
            /* Tiny random walk. Each thread derives a deterministic-ish local seed. */
            unsigned int local = *seed ^ (unsigned int)(i * 2654435761u) ^ (unsigned int)tick;
            int dir = rng_int(&local, 4);
            if (dir == 0 && drivers[i].x < grid_size - 1) drivers[i].x += 1.0;
            if (dir == 1 && drivers[i].x > 0) drivers[i].x -= 1.0;
            if (dir == 2 && drivers[i].y < grid_size - 1) drivers[i].y += 1.0;
            if (dir == 3 && drivers[i].y > 0) drivers[i].y -= 1.0;
        }
    }
}

int drivers_count_idle(const Driver *drivers, int n) {
    int count = 0;
    for (int i = 0; i < n; ++i) count += (drivers[i].status == DRIVER_IDLE);
    return count;
}
