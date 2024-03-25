#pragma once

#include <cstdlib>
#include <limits>
#include <cstdint>
#include <sys/stat.h>
#include <typeinfo>
#include "../include/parameters.h"

#define LOADSCALE 8
#define KILO 1024
#define MILLION (KILO * KILO)
#define INDEXNAME "PlinIndex"


const _key_t MAX_KEY = std::numeric_limits<_key_t>::max();
const _key_t MIN_KEY = typeid(_key_t) == typeid(double) || typeid(_key_t) == typeid(float) 
                            ? -1 * MAX_KEY : std::numeric_limits<_key_t>::min();

enum DistributionType {RAND = 0, ZIPFIAN};
enum OperationType {READ = 0, INSERT, UPDATE, DELETE};

struct QueryType {
    OperationType op;
    int64_t key;
};

#include <sys/time.h>
inline double seconds()
{
  timeval now;
  gettimeofday(&now, NULL);
  return now.tv_sec + now.tv_usec/1000000.0;
}

inline int getRandom() {
    timeval now;
    gettimeofday(&now, NULL);
    return now.tv_usec;
}

inline bool file_exist(const char *pool_path) {
    struct stat buffer;
    return (stat(pool_path, &buffer) == 0);
}


