/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   TextElement.hpp
 * @author Simon Brummer
 * @date   14.6.2017
 */

#ifndef TEXTELEMENT_HPP_201706140914
#define TEXTELEMENT_HPP_201706140914

#include <memory>
#include <cstdint>
#include <curses.h>
#include "BaseElement.hpp"

/**
 * @brief Element with static text.
 */
class TextElement : public BaseElement
{
public:
    /**
     * @brief Fully specified constructor.
     * @param[in] originX   x-coordinate element origin.
     * @param[in] originY   y-coordinate element origin.
     * @param[in] content   Content of the text element.
     */
    TextElement(std::size_t originX, std::size_t originY, std::string const& content)
        : BaseElement(originX, originY)
        , content(content)
    {
    }

    /**
     * @brief Destructor
     */
    virtual ~TextElement()
    {
    }

    /**
     * @brief Print TextElement
     * @param[in] wnd   Pointer to Window this Element should be drawn in.
     */
    virtual auto draw(WINDOW *wnd) -> void const
    {
       BaseElement::draw(wnd);
       wprintw(wnd, "%s", content.c_str());
    }

protected:
    std::string content; // Content of the Text Element
};

/**
 * @brief Generator function for TextElements.
 * @param[in] x         x-coordinate element origin.
 * @param[in] y         y-coordinate element origin.
 * @param[in] content   Content of the text element.
 * @returns   Shared pointer of Type BaseElement
 */
auto makeTextElement(size_t x, size_t y, std::string content) -> std::shared_ptr<BaseElement>
{
    return std::make_shared<TextElement>(x,y,content);
}


#endif // TEXTELEMENT_HPP_201706140914
