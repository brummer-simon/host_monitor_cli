/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   ObserverElement.cpp
 * @author Simon Brummer
 * @date   08.04.2018
 */

#include <cstring>
#include "Constants.hpp"
#include "Util.hpp"
#include "ObserverElement.hpp"

ObserverElement::ObserverElement( ConfigHost const&                          host
                                , std::vector<ConfigGlobal::FieldFmt> const& fmt
                                , std::mutex&                                mtx
                                , std::condition_variable&                   cv
                                , std::atomic_bool&                          redraw_ui)
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

void ObserverElement::draw(Window::Pointer wnd, Position& pos)
{
    // Calculate number of left characters, prevent underflow
    auto chars_left = std::int16_t(0);
    chars_left = wnd->get_width() - (2 * (ui_border_width + ui_line_offset_x));
    chars_left = (chars_left < 0) ? 0 : chars_left;

    wnd->add_string(content_, chars_left);

    pos.x += content_.size();
    wnd->move_to(pos);

    // Remove consumend characters, again check for underflow
    chars_left -= content_.size();
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

auto ObserverElement::get_height() -> std::uint16_t
{
    return ui_observer_elem_height;
}

auto ObserverElement::get_width() -> std::uint16_t
{
    return content_.size() + std::max( std::strlen(ui_status_available)
                                     , std::strlen(ui_status_unavailable));
}

void ObserverElement::state_change( Endpoint const&             endpoint
                                  , std::vector<uint8_t> const& metadata
                                  , std::chrono::seconds const& interval
                                  , bool                        available)
{
    // Surpress compiler warnings.
    (void) endpoint;
    (void) metadata;
    (void) interval;

    // State change occured.
    // Update internal state and notify ui thread to redraw ui.
    auto lock = std::unique_lock<std::mutex>(mtx_);
    available_ = available;
    redraw_ui_= true;
    cv_.notify_one();
}