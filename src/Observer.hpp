/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   Observer.hpp
 * @author Simon Brummer
 * @date   14.6.2017
 */

#ifndef OBSERVER_HPP_201706140914
#define OBSERVER_HPP_201706140914

#include <memory>
#include <HostMonitor.hpp>

using namespace host_monitor;

/**
 * @brief Observer object for registration on HostMonitor Objects.
 *        Stores current connection status.
 */
class Observer : public HostMonitorObserver
{
public:
    Observer(bool avail)
        : avail_(avail)
    {
    }

    virtual ~Observer()
    {
    }

    virtual auto update(bool targetAvail) -> void
    {
        avail_ = targetAvail;
    }

    virtual auto avail() const -> bool
    {
        return avail_;
    }

private:
    bool avail_;
};

/**
 * @brief Creates observer and register observer at the given monitor.
 * @param[in] monitor   the monitor, the created observer registers on.
 */
auto makeObserver(std::shared_ptr<HostMonitor> const& monitor) -> std::shared_ptr<HostMonitorObserver>
{
	std::shared_ptr<HostMonitorObserver> obs = std::make_shared<Observer>(monitor->avail());
    monitor->addObserver(obs);
    return obs;
}

#endif // OBSERVER_HPP_201706140914
