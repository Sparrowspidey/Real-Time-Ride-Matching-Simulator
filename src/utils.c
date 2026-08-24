#define _POSIX_C_SOURCE 200809L
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

double now_seconds(void) {
#ifdef _WIN32
    static LARGE_INTEGER freq;
    static int initialized = 0;
    LARGE_INTEGER counter;
    if (!initialized) { QueryPerformanceFrequency(&freq); initialized = 1; }
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)freq.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
#endif
}

unsigned int rng_next(unsigned int *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}
double rng_unit(unsigned int *state) { return rng_next(state) / 4294967296.0; }
int rng_int(unsigned int *state, int max_exclusive) {
    return max_exclusive > 0 ? (int)(rng_next(state) % (unsigned int)max_exclusive) : 0;
}

void config_defaults(Config *cfg) {
    cfg->grid_size = 100;
    cfg->num_drivers = 1000;
    cfg->num_riders = 500;
    cfg->ticks = 200;
    cfg->seed = 42;
    cfg->max_trip_ticks = 5;
    cfg->request_probability = 0.25;
}

void usage(const char *p) {
    printf("Usage: %s [--drivers N] [--riders N] [--ticks N] [--grid N] [--seed N] [--request-prob P]\n", p);
}

int parse_args(Config *cfg, int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        if (i + 1 >= argc) { usage(argv[0]); return 0; }
        if (!strcmp(argv[i], "--drivers")) cfg->num_drivers = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--riders")) cfg->num_riders = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--ticks")) cfg->ticks = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--grid")) cfg->grid_size = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--seed")) cfg->seed = (unsigned int)strtoul(argv[++i], NULL, 10);
        else if (!strcmp(argv[i], "--request-prob")) cfg->request_probability = atof(argv[++i]);
        else { usage(argv[0]); return 0; }
    }
    if (cfg->num_drivers <= 0 || cfg->num_riders <= 0 || cfg->ticks <= 0 || cfg->grid_size <= 1) return 0;
    if (cfg->request_probability < 0.0 || cfg->request_probability > 1.0) return 0;
    return 1;
}
