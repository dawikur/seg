// Copyright 2017, Dawid Kurek, <dawikur@gmail.com>

#include "lel_benchmark.hpp"

void noop(::benchmark::State &state) {
  while (state.KeepRunning())
    ;
}

//BENCHMARK(noop);

BENCHMARK_MAIN()
