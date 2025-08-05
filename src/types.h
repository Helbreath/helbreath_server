//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#pragma once
#include <cstdint>
#include <string>
#include <vector>

enum class manager_status_t
{
    CONNECTED = 1,
    AUTHENTICATED = 2,
};

enum class client_status_t
{
    LOGIN_SCREEN = 1,
    CONNECTING_TO_GAME,
    PLAYING, // misc status?
};

enum class character_status_t
{
    ALIVE = 1,
    DEAD = 2,
};

enum class server_status_t
{
    UNINITIALIZED = 0,
    RUNNING,
    RUNNING_QUEUE,
    SHUTDOWN,
    OFFLINE,
};

struct RECT
{
    RECT(int32_t a, int32_t b, int32_t c, int32_t d) { left = a; top = b; right = c; bottom = d; }
    RECT() { left = top = right = bottom = 0; };
    RECT(const RECT & r) { left = r.left; top = r.top; right = r.right; bottom = r.bottom; }
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
};

struct POINT
{
    POINT(int32_t a, int32_t b) { x = a; y = b; }
    POINT() { x = y = 0; };
    int32_t  x;
    int32_t  y;
};
