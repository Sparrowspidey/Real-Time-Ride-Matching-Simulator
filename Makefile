CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra -std=c11 -Iinclude
OMPFLAGS = -fopenmp
LDFLAGS = -lm

COMMON = src/utils.c src/geo.c src/driver.c src/rider.c src/matching.c src/metrics.c

.PHONY: all clean test
all: bin/simulator_serial bin/simulator_parallel

bin/simulator_serial: src/main.c $(COMMON) | bin
	$(CC) $(CFLAGS) -o $@ src/main.c $(COMMON) $(LDFLAGS)

bin/simulator_parallel: src/main.c $(COMMON) | bin
	$(CC) $(CFLAGS) $(OMPFLAGS) -DBUILD_PARALLEL=1 -o $@ src/main.c $(COMMON) $(OMPFLAGS) $(LDFLAGS)

bin:
	mkdir -p bin

test: tests/test_matching
	./tests/test_matching

tests/test_matching: tests/test_matching.c src/geo.c src/matching.c | tests
	$(CC) $(CFLAGS) $(OMPFLAGS) -o $@ tests/test_matching.c src/geo.c src/matching.c $(LDFLAGS)

clean:
	rm -f bin/simulator_serial bin/simulator_parallel tests/test_matching
