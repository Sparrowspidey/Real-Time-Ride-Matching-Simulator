#ifndef METRICS_H
#define METRICS_H

typedef struct {
    long long total_matches;
    long long total_wait_ticks;
    long long total_driver_busy_ticks;
    long long total_driver_observations;
} Metrics;

void metrics_init(Metrics *m);
void metrics_record_matches(Metrics *m, long long matches, long long wait_ticks);
void metrics_record_utilization(Metrics *m, int busy_drivers, int total_drivers);
void metrics_print(const Metrics *m);

#endif
