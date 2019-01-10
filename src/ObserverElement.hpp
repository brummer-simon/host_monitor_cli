/**
 * @file      ObserverElement.hpp
 * @author    Simon Brummer (<simon.brummer@posteo.de>)
 * @brief     Command line argument parsing.
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef OBSERVERELEMENT_HPP_201804081223
#define OBSERVERELEMENT_HPP_201804081223

#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <cstdint>
#include <host_monitor/HostMonitor.hpp>
#include <host_monitor/HostMonitorObserver.hpp>
#include "Config.hpp"
#include "Element.hpp"

using host_monitor::HostMonitorObserver;
using host_monitor::Endpoint;

// Ui element that can be registered as observer on the host monitor.
class ObserverElement : public Element, public HostMonitorObserver
{
public:
    using Pointer = std::shared_ptr<ObserverElement>;

    // Constructor: @p mtx, @p cv, @p redraw_ui are used for synchronization
    //              with the main thread. @p host and @p fmt is used to
    //              draw the ui element contents.
    ObserverElement( ConfigHost const&                          host
                   , std::vector<ConfigGlobal::FieldFmt> const& fmt
                   , std::mutex&                                mtx
                   , std::condition_variable&                   cv
                   , std::atomic_bool&                          redraw_ui
                   );

    virtual ~ObserverElement() = default;

    // Element Interface interface implementation
    virtual void draw(Window::Pointer wnd, Position& pos) const override;
    virtual unsigned get_height() const override ;
    virtual unsigned get_width() const override ;

    // HostMonitorObserver interface implementation. Executed in the thread
    // context of host monitor
    virtual void state_change(HostMonitorObserver::Data const& data) override;

private:
    std::string              content_;
    std::atomic_bool         available_;

    // For synchronization with main thread
    std::mutex&              mtx_;
    std::condition_variable& cv_;
    std::atomic_bool&        redraw_ui_;
};

#endif // OBSERVERELEMENT_HPP_201804081223
