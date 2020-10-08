#include "time_check.h"

unsigned long long currentTimeNano()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (long long)(t.tv_sec * GIGA_MODIFIER + t.tv_nsec);
}

unsigned long long currentTimeMillis()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (long long)(t.tv_sec * KILO_MODIFIER + t.tv_nsec * MICRO_MODIFIER);
}