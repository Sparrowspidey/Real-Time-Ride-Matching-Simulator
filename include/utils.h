#ifndef UTILS_H
#define UTILS_H

#include "types.h"

double now_seconds(void);
void config_defaults(Config *cfg);
int parse_args(Config *cfg, int argc, char **argv);
unsigned int rng_next(unsigned int *state);
double rng_unit(unsigned int *state);
int rng_int(unsigned int *state, int max_exclusive);
void usage(const char *program);

#endif
