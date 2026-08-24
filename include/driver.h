#ifndef DRIVER_H
#define DRIVER_H

#include "types.h"

void drivers_init(Driver *drivers, int n, int grid_size, unsigned int seed);
void drivers_update(Driver *drivers, int n, int grid_size, int tick, unsigned int *seed);
int drivers_count_idle(const Driver *drivers, int n);

#endif
