#include "rider.h"
#include "utils.h"

int rider_generate(Rider *riders, int capacity, int *count, int tick,
                   int grid_size, double request_probability, unsigned int *seed) {
    if (*count >= capacity || rng_unit(seed) > request_probability) return 0;

    Rider *r = &riders[*count];
    r->id = *count;
    r->x = (double)rng_int(seed, grid_size);
    r->y = (double)rng_int(seed, grid_size);
    r->request_tick = tick;
    r->matched_driver_id = -1;
    r->matched_tick = -1;
    r->wait_ticks = 0;
    (*count)++;
    return 1;
}
