/**
 * @file      GroupElement.cpp
 * @author    Simon Brummer (<simon.brummer@posteo.de>)
 * @brief     Command line argument parsing.
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "Constants.hpp"
#include "GroupElement.hpp"

GroupElement::GroupElement ( std::optional<std::string> const&            name
                           , std::vector<ObserverElement::Pointer> const& observers
                           )
    : name_(name)
    , observers_(observers)
{
}

void GroupElement::draw(Window::Pointer wnd, Position& pos) const
{
    // Draw group name in case there is one.
    if (name_)
    {
        // Calculate number of left characters, prevent underflow
        auto chars_left = 0;
        chars_left = wnd->get_width() - (2 * (ui_border_width + ui_line_offset_x));
        chars_left = (chars_left < 0) ? 0 : chars_left;

        pos = Position( ui_border_width + ui_line_offset_x
                      , pos.y + ui_line_offset_y
                      );

        wnd->move_to(pos);
        wnd->set_underlined();
        wnd->add_string(name_.value(), chars_left);
        wnd->unset_underlined();
    }

    // Draw Groups Elements
    for (auto obs : observers_)
    {
        pos = Position( ui_border_width + ui_line_offset_x
                      , pos.y + ui_line_offset_y
                      );

        wnd->move_to(pos);
        obs->draw(wnd, pos);
    }
}

unsigned GroupElement::get_height() const
{
    auto height = static_cast<unsigned>(observers_.size());

    // In case the groups has a name: reserve a line for it.
    if (name_)
    {
        height += 1;
    }
    return height;
}

unsigned GroupElement::get_width() const
{
    auto width = static_cast<unsigned>(name_.value_or("").size());

    for (auto obs : observers_)
    {
        width = std::max(width, obs->get_width());
    }
    return width + 2 * ui_line_offset_x;
}
