#include <assert.h>
#include <stdio.h>
#include "types.h"
#include "matching.h"

int main(void) {
    Driver drivers[2] = {
        {.id=0, .x=0, .y=0, .status=DRIVER_IDLE, .busy_until_tick=0},
        {.id=1, .x=10, .y=10, .status=DRIVER_IDLE, .busy_until_tick=0}
    };
    Rider riders[2] = {
        {.id=0, .x=1, .y=1, .request_tick=0, .matched_driver_id=-1},
        {.id=1, .x=9, .y=9, .request_tick=0, .matched_driver_id=-1}
    };
    long long matches = match_serial(drivers, 2, riders, 2, 0);
    assert(matches == 2);
    assert(riders[0].matched_driver_id == 0);
    assert(riders[1].matched_driver_id == 1);
    printf("test_matching: PASS\n");
    return 0;
}
