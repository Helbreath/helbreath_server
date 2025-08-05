//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include <ctime>
#include <cstdint>
#include <csignal>
#include <mutex>
#include "game.h"

char G_cTxt[512];
char G_cData50000[50000];

CGame * game;

static void signal_handler(int signum)
{
    if (signum == SIGINT)
    {
        std::lock_guard<std::mutex> lock(server->cv_mtx);
        server->set_server_state(server_status_t::SHUTDOWN);
        server->cv_exit.notify_one();
    }
}

int main(int argc, char * argv[])
{
    srand((uint32_t)time(nullptr));

    struct sigaction sa {};
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, nullptr);

    game = new CGame();

    try
    {
        game->run();
    }
    catch (const std::exception & e)
    {
        game->log->critical("Unhandled exception: {}", e.what());
    }
    catch (...)
    {
        game->log->critical("Unhandled exception: unknown");
    }

    return 0;
}
