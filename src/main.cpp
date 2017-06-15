/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   main.cpp
 * @author Simon Brummer
 * @date   14.6.2017
 */

#include <thread>
#include <vector>
#include <memory>
#include <iostream>
#include <signal.h>

#include "ui/Ui.hpp"
#include "ui/Terminal.hpp"
#include "config/Config.hpp"
#include "HostMonitor.hpp"
#include "Observer.hpp"

using namespace host_monitor;

/* Constants */
namespace
{
    size_t const                    refreshRate = 30; // In Hz
    std::chrono::milliseconds const sleepDuration(1000 / refreshRate);
}

/* Variables */
namespace
{
    bool shutdown = false;
}

/**
 * @brief Signal Handler for Crtl+c, initiates clean shutdown.
 */
void sigHandler(int sig)
{
    if (sig == SIGINT)
    {
        shutdown = true;
    }
}

int main(int argc, char **argv)
{
    // Register signal handler
    signal(SIGINT, sigHandler);

    // Parse given arguments and read config file
    auto args = readArgs(argc, argv);
    auto monitors = readConfigFile(args["-f"]);

    // Create Observers based on read HostMonitors
    std::vector<std::shared_ptr<HostMonitorObserver>> observers;
    for (auto it = monitors.begin(); it != monitors.end(); ++it)
    {
        observers.push_back(makeObserver(*it));
    }

    // Create user interface
    Terminal term;
    auto ui = makeUi(monitors, observers, term.getWidth(), term.getHeight());

    // Loop until termination signal is received
    while (!shutdown)
    {
        // Check if terminal height and width changed. If so: re-generate UI
        std::size_t lastTermWidth = term.getWidth();
        std::size_t lastTermHeight = term.getHeight();
        term.refreshTermDimensions();

        if ((term.getWidth() != lastTermWidth) || (term.getHeight() != lastTermHeight))
        {
            term.reset();
            ui = makeUi(monitors, observers, term.getWidth(), term.getHeight());
        }

        // Draw user interface, sleep afterwards
        ui->draw();
        std::this_thread::sleep_for(sleepDuration);
    }

    // Cleanup: Unregister observers
    ui.reset();
    for (size_t i = 0; i < monitors.size(); ++i)
    {
        monitors[i]->delObserver(observers[i]);
    }
    return 0;
}
