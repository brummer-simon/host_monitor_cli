/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   BaseElement.hpp
 * @author Simon Brummer
 * @date   14.6.2017
 */

#ifndef BASEELEMENT_HPP_201706140914
#define BASEELEMENT_HPP_201706140914

#include <cstdint>
#include <curses.h>

/**
 * @brief Most basic element. Every Element must derive from this class.
 */
class BaseElement
{
public:
    /**
     * @brief Fully specified constructor.
     * @param[in] originX   x-coordinate element origin.
     * @param[in] originY   y-coordinate element origin.
     */
    BaseElement(std::size_t originX, std::size_t originY)
        : originX(originX)
        , originY(originY)
    {
    }

    /**
     * @brief Destructor
     */
    virtual ~BaseElement()
    {
    }

    /**
     * @brief Moves cursor to BaseElements origin.
     * @param[in] wnd   Pointer to Window this Element should be drawn in.
     */
    virtual auto draw(WINDOW *wnd) -> void const
    {
        wmove(wnd, originY, originX);
    }

protected:
    std::size_t originX; // Element origin x-coordinate (relative to window containing element)
    std::size_t originY; // Element origin y-coordinate (relative to window containing element)
};

#endif // BASEELEMENT_HPP_201706140914
