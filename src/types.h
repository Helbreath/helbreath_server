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
