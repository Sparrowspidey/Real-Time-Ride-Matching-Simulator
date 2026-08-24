#include "metrics.h"
#include <stdio.h>

void metrics_init(Metrics *m) { *m = (Metrics){0}; }
void metrics_record_matches(Metrics *m, long long matches, long long wait_ticks) {
    m->total_matches += matches;
    m->total_wait_ticks += wait_ticks;
}
void metrics_record_utilization(Metrics *m, int busy_drivers, int total_drivers) {
    m->total_driver_busy_ticks += busy_drivers;
    m->total_driver_observations += total_drivers;
}
void metrics_print(const Metrics *m) {
    double avg_wait = m->total_matches ? (double)m->total_wait_ticks / m->total_matches : 0.0;
    double utilization = m->total_driver_observations ?
        (double)m->total_driver_busy_ticks / m->total_driver_observations : 0.0;
    printf("matches=%lld avg_wait_ticks=%.3f utilization=%.3f\n",
           m->total_matches, avg_wait, utilization);
}
