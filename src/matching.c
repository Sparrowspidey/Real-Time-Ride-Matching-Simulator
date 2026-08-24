#include "matching.h"
#include "geo.h"
#include <float.h>
#include <stdlib.h>

long long match_serial(Driver *drivers, int driver_count, Rider *riders, int rider_count, int tick) {
    long long matches = 0;
    for (int r = 0; r < rider_count; ++r) {
        double best = DBL_MAX;
        int best_driver = -1;
        for (int d = 0; d < driver_count; ++d) {
            if (drivers[d].status != DRIVER_IDLE) continue;
            double dist = distance_sq(riders[r].x, riders[r].y, drivers[d].x, drivers[d].y);
            if (dist < best) { best = dist; best_driver = d; }
        }
        if (best_driver >= 0) {
            drivers[best_driver].status = DRIVER_BUSY;
            drivers[best_driver].busy_until_tick = tick + 5;
            riders[r].matched_driver_id = drivers[best_driver].id;
            riders[r].matched_tick = tick;
            riders[r].wait_ticks = tick - riders[r].request_tick;
            matches++;
        }
    }
    return matches;
}

long long match_parallel(Driver *drivers, int driver_count, Rider *riders, int rider_count, int tick) {
    int *candidate = malloc((size_t)rider_count * sizeof(*candidate));
    if (!candidate) return -1;

    #pragma omp parallel for schedule(static)
    for (int r = 0; r < rider_count; ++r) {
        double best = DBL_MAX;
        int best_driver = -1;
        for (int d = 0; d < driver_count; ++d) {
            if (drivers[d].status != DRIVER_IDLE) continue;
            double dist = distance_sq(riders[r].x, riders[r].y, drivers[d].x, drivers[d].y);
            if (dist < best) { best = dist; best_driver = d; }
        }
        candidate[r] = best_driver;
    }

    /* Deterministic conflict resolution. Only this narrow section is serial. */
    long long matches = 0;
    for (int r = 0; r < rider_count; ++r) {
        int d = candidate[r];
        if (d < 0 || drivers[d].status != DRIVER_IDLE) continue;
        drivers[d].status = DRIVER_BUSY;
        drivers[d].busy_until_tick = tick + 5;
        riders[r].matched_driver_id = drivers[d].id;
        riders[r].matched_tick = tick;
        riders[r].wait_ticks = tick - riders[r].request_tick;
        matches++;
    }
    free(candidate);
    return matches;
}
