#ifndef MATCHING_H
#define MATCHING_H

#include "types.h"

long long match_serial(Driver *drivers, int driver_count, Rider *riders, int rider_count, int tick);
long long match_parallel(Driver *drivers, int driver_count, Rider *riders, int rider_count, int tick);

#endif
