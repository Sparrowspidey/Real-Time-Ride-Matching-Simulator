#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "driver.h"
#include "rider.h"
#include "matching.h"
#include "metrics.h"
#include "utils.h"

#ifndef BUILD_PARALLEL
#define BUILD_PARALLEL 0
#endif

static int run_simulation(const Config *cfg) {
    Driver *drivers = calloc((size_t)cfg->num_drivers, sizeof(*drivers));
    Rider *riders = calloc((size_t)cfg->num_riders, sizeof(*riders));
    if (!drivers || !riders) { free(drivers); free(riders); return 1; }

    unsigned int seed = cfg->seed;
    drivers_init(drivers, cfg->num_drivers, cfg->grid_size, seed);
    Metrics metrics;
    metrics_init(&metrics);
    int rider_count = 0;
    double matching_seconds = 0.0;

    for (int tick = 0; tick < cfg->ticks; ++tick) {
        drivers_update(drivers, cfg->num_drivers, cfg->grid_size, tick, &seed);
        rider_generate(riders, cfg->num_riders, &rider_count, tick, cfg->grid_size,
                       cfg->request_probability, &seed);

        if (rider_count > 0) {
            double start = now_seconds();
#if BUILD_PARALLEL
            long long matches = match_parallel(drivers, cfg->num_drivers, riders, rider_count, tick);
#else
            long long matches = match_serial(drivers, cfg->num_drivers, riders, rider_count, tick);
#endif
            matching_seconds += now_seconds() - start;
            if (matches < 0) { free(drivers); free(riders); return 1; }

            long long wait_ticks = 0;
            for (int r = 0; r < rider_count; ++r)
                if (riders[r].matched_driver_id >= 0) wait_ticks += riders[r].wait_ticks;
            metrics_record_matches(&metrics, matches, wait_ticks);
        }

        int busy = cfg->num_drivers - drivers_count_idle(drivers, cfg->num_drivers);
        metrics_record_utilization(&metrics, busy, cfg->num_drivers);
    }

    printf("mode=%s drivers=%d riders=%d ticks=%d\n",
#if BUILD_PARALLEL
           "parallel",
#else
           "serial",
#endif
           cfg->num_drivers, cfg->num_riders, cfg->ticks);
    printf("matching_time_seconds=%.6f\n", matching_seconds);
    metrics_print(&metrics);

    free(drivers);
    free(riders);
    return 0;
}

int main(int argc, char **argv) {
    Config cfg;
    config_defaults(&cfg);
    if (!parse_args(&cfg, argc, argv)) return EXIT_FAILURE;
    return run_simulation(&cfg) ? EXIT_FAILURE : EXIT_SUCCESS;
}
