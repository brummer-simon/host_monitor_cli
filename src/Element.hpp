/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   Element.hpp
 * @author Simon Brummer
 * @date   14.6.2017
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
    virtual void draw(Window::Pointer wnd, Position& pos) = 0;

    // Get height of element in characters
    virtual auto get_height() -> std::uint16_t = 0;

    // Get width of element in characters
    virtual auto get_width() -> std::uint16_t = 0;
};

#endif // ELEMENT_HPP_201706140914
