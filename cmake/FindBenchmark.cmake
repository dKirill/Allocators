# Try to find the Google Benchmark library and headers.
#  Benchmark_FOUND        - system has benchmark lib
#  Benchmark_INCLUDE_DIRS - the benchmark include directory
#  Benchmark_LIBRARIES    - libraries needed to use benchmark
#  Benchmark_MAIN_LIBRARIES
#  Benchmark_BOTH_LIBRARIES

find_path(Benchmark_INCLUDE_DIR
  NAMES benchmark/benchmark.h
  HINTS
    $ENV{Benchmark_ROOT}/include
    ${Benchmark_ROOT}/include
  NO_SYSTEM_ENVIRONMENT_PATH
  DOC "The directory where benchmark includes reside"
)

find_library(Benchmark_LIBRARY
  NAMES benchmark
  HINTS
    $ENV{Benchmark_ROOT}/lib
    ${Benchmark_ROOT}/lib
  NO_SYSTEM_ENVIRONMENT_PATH
  DOC "The benchmark library"
)

find_library(Benchmark_MAIN_LIBRARY
  NAMES benchmark_main
  HINTS
    $ENV{Benchmark_ROOT}/lib
    ${Benchmark_ROOT}/lib
  NO_SYSTEM_ENVIRONMENT_PATH
  DOC "The benchmark main library"
)

set(Benchmark_INCLUDE_DIRS ${Benchmark_INCLUDE_DIR})
set(Benchmark_LIBRARIES    ${Benchmark_LIBRARY})
set(Benchmark_MAIN_LIBRARIES    ${Benchmark_MAIN_LIBRARY})
set(Benchmark_BOTH_LIBRARIES    ${Benchmark_LIBRARIES} ${Benchmark_MAIN_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Benchmark
  FOUND_VAR Benchmark_FOUND
  REQUIRED_VARS Benchmark_INCLUDE_DIR Benchmark_LIBRARY
)

mark_as_advanced(Benchmark_FOUND)
