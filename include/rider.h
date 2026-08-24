#ifndef RIDER_H
#define RIDER_H

#include "types.h"

int rider_generate(Rider *riders, int capacity, int *count, int tick,
                   int grid_size, double request_probability, unsigned int *seed);

#endif
