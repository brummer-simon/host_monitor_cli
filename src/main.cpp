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

#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <csignal>
#include <host_monitor/HostMonitor.hpp>
#include "Args.hpp"
#include "Config.hpp"
#include "Util.hpp"
#include "UserInterface.hpp"
#include "GroupElement.hpp"
#include "ObserverElement.hpp"

using namespace host_monitor;

using sec = std::chrono::seconds;
using msec = std::chrono::milliseconds;
using MonitorPtr = std::shared_ptr<HostMonitor>;
using ObserverPtr = std::shared_ptr<HostMonitorObserver>;
using MonitorObserver = std::pair<MonitorPtr, ObserverPtr>;
using Proto = Endpoint::Protocol;

namespace
{
    // Workaround to use capture list in signal handling
    std::function<void(int)> signal_handler;
}

int main(int argc, char **argv)
{
    // Synchronization primitives
    auto mtx = std::mutex();
    auto cv = std::condition_variable();

    auto shutdown_ui = std::atomic_bool(false);
    auto rebuild_ui = std::atomic_bool(false);
    auto redraw_ui = std::atomic_bool(true);

    // Setup Signal Handling
    signal_handler = [&mtx, &cv, &shutdown_ui, &rebuild_ui] (int signo)
    {
        switch(signo)
        {
            case SIGINT:
            {
                auto lock = std::unique_lock<std::mutex>(mtx);
                shutdown_ui = true;
                cv.notify_one();
                break;
            }

            case SIGWINCH:
            {
                auto lock = std::unique_lock<std::mutex>(mtx);
                rebuild_ui = true;
                cv.notify_one();
                break;
            }
            default:
                return;
        }
    };
    std::signal(SIGINT,   [] (int signo) {signal_handler(signo);} );
    std::signal(SIGWINCH, [] (int signo) {signal_handler(signo);} );

    // Parse given arguments and read config file
    auto args = read_args(argc, argv);
    auto config = read_config_file(args["-f"]);

    auto attached_observers = std::vector<MonitorObserver>();
    auto group_elements = std::vector<GroupElement::Pointer>();

    // Setup Groups and Monitoring
    for (auto grp : config.groups)
    {
        auto observers = std::vector<ObserverElement::Pointer>();

        // Make host monitors and associated observers from this config group
        for (auto const& host : grp.hosts)
        {
            // Create Host Monitor and Observers
            auto p = string_to_protocol(host.protocol);
            auto endpoint = (p.value() == Proto::ICMP) ? Endpoint::make_icmp_endpoint(host.fqhn)
                                                       : Endpoint::make_tcp_endpoint(host.fqhn, host.port.value());

            auto interval = sec(string_to_int(host.interval).value());

            auto monitor = std::make_shared<HostMonitor>(endpoint, interval);

            auto observer = std::make_shared<ObserverElement>( host
                                                             , config.global.field_format
                                                             , mtx
                                                             , cv
                                                             , redraw_ui);

            // Attach observer and store association for later cleanup
            monitor->add_observer(observer);
            attached_observers.push_back(MonitorObserver(monitor, observer));
            observers.push_back(observer);
        }

        // Create ui group from config group
        group_elements.push_back(std::make_shared<GroupElement>(grp.name, observers));
    }

    // Setup and run curses ui
    auto ui = UserInterface(group_elements, config.global.field_format);

    // Main thread processing loop.
    while (shutdown_ui != true)
    {
        // Rebuild ui (caused by terminal resize), implies redraw.
        if (rebuild_ui)
        {
            rebuild_ui = false;
            ui.rebuild_ui();
            redraw_ui = true;
        }

        // Internal state of the shown observers changed. Redraw ui
        if (redraw_ui)
        {
            redraw_ui = false;
            ui.draw();
        }

        // Wait until any of the following conditions is true
        // 1) Shutdown is true (set by signal handler)
        // 2) ui must be rebuilt (set by signal handler)
        // 3) ui must be redrawn (set by observer state change)
        auto lock = std::unique_lock<std::mutex>(mtx);
        auto cond = [&shutdown_ui, &rebuild_ui, &redraw_ui]
        {
            return (shutdown_ui || rebuild_ui || redraw_ui);
        };
        cv.wait(lock, cond);
    }

    // Cleanup: Detach attached Observers
    for (auto [monitor, observer] : attached_observers)
    {
        monitor->del_observer(observer);
    }
    return 0;
}
