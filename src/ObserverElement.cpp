/**
 * @file      ObserverElement.cpp
 * @author    Simon Brummer (<simon.brummer@posteo.de>)
 * @brief     Command line argument parsing.
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <cstring>
#include "Constants.hpp"
#include "Util.hpp"
#include "ObserverElement.hpp"

ObserverElement::ObserverElement( ConfigHost const&                          host
                                , std::vector<ConfigGlobal::FieldFmt> const& fmt
                                , std::mutex&                                mtx
                                , std::condition_variable&                   cv
                                , std::atomic_bool&                          redraw_ui
                                )
   : content_()
   , available_(false)
   , mtx_(mtx)
   , cv_(cv)
   , redraw_ui_(redraw_ui)
{
    // Construct String to draw from given format.
    auto str = std::string();

    for (auto const& [type, len] : fmt)
    {
        switch(type)
        {
            case Field::Fqhn:
                append_and_fill(str, host.fqhn, len);
                break;

            case Field::Alias:
                append_and_fill(str, host.alias.value_or(""), len);
                break;

            case Field::Role:
                append_and_fill(str, host.role.value_or(""), len);
                break;

            case Field::Device:
                append_and_fill(str, host.device.value_or(""), len);
                break;

            case Field::Protocol:
                append_and_fill(str, make_proto_port_string(host.protocol, host.port), len);
                break;

            case Field::Interval:
                append_and_fill(str, make_interval_string(host.interval), len);
                break;

            default:
                append_and_fill(str, "", len);
        }

        // Append spacer between fields
        str.append(ui_field_space);
    }
    content_ = std::move(str);
}

void ObserverElement::draw(Window::Pointer wnd, Position& pos) const
{
    // Calculate number of left characters, prevent underflow
    auto chars_left = 0;
    chars_left = wnd->get_width() - (2 * (ui_border_width + ui_line_offset_x));
    chars_left = (chars_left < 0) ? 0 : chars_left;

    wnd->add_string(content_, chars_left);

    pos.x += static_cast<unsigned>(content_.size());
    wnd->move_to(pos);

    // Remove consumed characters, again check for underflow
    chars_left -= static_cast<int>(content_.size());
    chars_left = (chars_left < 0) ? 0 : chars_left;

    if (available_)
    {
        wnd->set_foreground_color(Window::Color::Green);
        wnd->add_string( std::string(ui_status_available)
                       , chars_left);
    }
    else
    {
        wnd->set_foreground_color(Window::Color::Red);
        wnd->add_string( std::string(ui_status_unavailable)
                       , chars_left);
    }
    wnd->unset_color();
}

unsigned ObserverElement::get_height() const
{
    return ui_observer_elem_height;
}

unsigned ObserverElement::get_width() const
{
    auto width = static_cast<unsigned>(content_.size());

    return width + static_cast<unsigned>(std::max( std::strlen(ui_status_available)
                                                 , std::strlen(ui_status_unavailable)
                                                 )
                                        );
}

void ObserverElement::state_change(HostMonitorObserver::Data const& data)
{
    // State change occured.
    // Update internal state and notify ui thread to redraw ui.
    auto lock = std::unique_lock<std::mutex>(mtx_);
    available_ = data.available;
    redraw_ui_= true;
    cv_.notify_one();
}
