#pragma once

#if defined(_MSC_VER)
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <cstdio>
#include <random>
#include <string>
#include <vector>
#include <set>
#include "xxfl_qstring.h"

typedef xxfl::qstring<false> xxfl_qstring32;
typedef xxfl::qstring<true>  xxfl_qstring64;

extern std::mt19937 rand_gen;

void time_start();
double get_elapsed_time();

template<typename _string>
_string gen_random_string()
{
    const uint32_t length_min = 6;
    const uint32_t length_max = 32;

    char buf[64];
    uint32_t length = length_min + (rand_gen() % (length_max + 1 - length_min));

    for (uint32_t i = 0; i < length; ++i)
    {
        buf[i] = '0' + (rand_gen() % 64);
    }

    return _string(buf, length);
}

template<typename _string>
void gen_random_string_vector(std::vector<_string>& x, uint32_t count)
{
    x.reserve(count);

    for (uint32_t i = 0; i < count; ++i)
    {
        x.push_back(gen_random_string<_string>());
    }
}
