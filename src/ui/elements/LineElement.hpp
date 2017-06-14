/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   LineElement.hpp
 * @author Simon Brummer
 * @date   14.6.2017
 */

#ifndef LINEELEMENT_HPP_201706141249
#define LINEELEMENT_HPP_201706141249

#include <memory>
#include <cstdint>
#include <curses.h>
#include "BaseElement.hpp"

/**
 * @brief Element with specifying a line.
 */
class LineElement : public BaseElement
{
public:
    /**
     * @brief Enumeration defining the orientation of a LineElement.
     */
    enum class Orientation
    {
        Vertical,
        Horizontal
    };

    /**
     * @brief Fully specified constructor.
     * @param[in] originX       x-coordinate element origin.
     * @param[in] originY       y-coordinate element origin.
     * @param[in] orientation   line orientation. Can be vertical or horizontal.
     * @param[in] length        line length measured in characters
     */
    LineElement(std::size_t originX, std::size_t originY, Orientation orientation, size_t length)
        : BaseElement(originX, originY)
        , orientation(orientation)
        , length(length)
    {
    }

    /**
     * @brief Destructor
     */
    virtual ~LineElement()
    {
    }

    /**
     * @brief Print TextElement
     * @param[in] wnd   Pointer to Window this Element should be drawn in.
     */
    virtual auto draw(WINDOW *wnd) -> void const
    {
        BaseElement::draw(wnd);
        switch(orientation)
        {
        case Orientation::Vertical:
            wvline(wnd, 0, length);
            break;

        case Orientation::Horizontal:
            whline(wnd, 0, length);
            break;
        }
    }

protected:
    Orientation orientation; // Orientation of the Line
    size_t      length;      // Length of the Line in characters
};

/**
 * @brief Generator function for LineElements.
 * @param[in] x   x-coordinate element origin.
 * @param[in] y   y-coordinate element origin.
 * @param[in] o   line orientation. Can be vertical or horizontal.
 * @param[in] l   line length measured in characters
 * @returns   Shared pointer of Type BaseElement
 */
auto makeLineElement(size_t x, size_t y, LineElement::Orientation o, size_t l
                    ) -> std::shared_ptr<BaseElement>
{
    return std::make_shared<LineElement>(x, y, o, l);
}

#endif // LINEELEMENT_HPP_201706141249
