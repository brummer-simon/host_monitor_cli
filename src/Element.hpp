/**
 * @file      Element.hpp
 * @author    Simon Brummer (<simon.brummer@posteo.de>)
 * @brief     Command line argument parsing.
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef ELEMENT_HPP_201706140914
#define ELEMENT_HPP_201706140914

#include <cstdint>
#include "Window.hpp"

// Simple interface for curses based ui elements
class Element
{
public:
    virtual ~Element() = default;

    // Draw ui element on @p wnd. @p pos is the last position
    virtual void draw(Window::Pointer wnd, Position& pos) const = 0;

    // Get height of element in characters
    virtual unsigned get_height() const = 0;

    // Get width of element in characters
    virtual unsigned get_width() const = 0;
};

#endif // ELEMENT_HPP_201706140914
