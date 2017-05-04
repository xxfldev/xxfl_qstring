#include "test_helper.h"

#if defined(_MSC_VER)
#include <windows.h>

static LARGE_INTEGER frequency = { 0, 0 };
static LARGE_INTEGER start_time;

void time_start()
{
    QueryPerformanceCounter(&start_time);
}

double get_elapsed_time()
{
    LARGE_INTEGER end_time;
    QueryPerformanceCounter(&end_time);

    if (frequency.QuadPart == 0)
    {
        QueryPerformanceFrequency(&frequency);
    }

    return double(end_time.QuadPart - start_time.QuadPart) / frequency.QuadPart;
}

#else
#include <sys/time.h>

static timeval start_time;

void time_start()
{
    gettimeofday(&start_time, nullptr);
}

double get_elapsed_time()
{
    timeval end_time;
    gettimeofday(&end_time, nullptr);
    return (double)(end_time.tv_sec - start_time.tv_sec) + (double)(end_time.tv_usec - start_time.tv_usec) / 1000000.0;
}
#endif
