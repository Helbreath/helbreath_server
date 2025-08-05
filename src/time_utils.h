//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#pragma once

#include <ctime>
#include <sys/timeb.h>
#include <chrono>

using namespace std::chrono;
using namespace std::chrono_literals;

inline time_point<system_clock> now() noexcept
{
    return system_clock::now();
}

template<typename A = seconds>
std::size_t time_count(nanoseconds a)
{
    return duration_cast<A>(a).count();
}

template<typename A = seconds>
std::size_t time_diff(time_point<steady_clock> a, time_point<steady_clock> b)
{
    return duration_cast<A>(a - b).count();
}

static inline int32_t timeGetTime()
{
    return (int32_t)duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
