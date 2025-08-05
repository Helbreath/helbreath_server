//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#pragma once

#include <cstdio>
#include <cstdarg>
#include <type_traits>

template<typename T>
auto copy_string(T & dest, const char * format, ...) -> std::enable_if_t<std::is_bounded_array<T>::value>
{
    constexpr std::size_t N = std::extent<T>::value;

    va_list args{};
    va_start(args, format);
    vsnprintf(dest, N, format, args);
    va_end(args);
}
