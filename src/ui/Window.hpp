/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   Window.hpp
 * @author Simon Brummer
 * @date   14.6.2017
 */

#ifndef WINDOW_HPP_201706140914
#define WINDOW_HPP_201706140914

#include "elements/Elements.hpp"

typedef std::shared_ptr<BaseElement> Element;

/**
 * @brief Class maintaining a single curses window.
 */
class Window
{
public:
    /**
     * @brief Fully specified constructor.
     * @param[in] x          Window origin x-coordinate
     * @param[in] y          Window origin y-coordinate
     * @param[in] width      Window width
     * @param[in] height     Window height
     * @param[in] elements   The elements this window contains
     */
    Window(std::size_t                                       x,
           std::size_t                                       y,
           std::size_t                                       width,
           std::size_t                                       height,
           std::vector<std::shared_ptr<BaseElement>> const & elements)
        : x_(x)
        , y_(y)
        , w_(width)
        , h_(height)
        , handle_(nullptr)
        , elements_(elements)
    {
        // Create Window, draw border
        handle_ = newwin(h_, w_, y_, x_);
    }

    /**
     * @brief Destructor
     */
    ~Window()
    {
        delwin(handle_);
    }

    /**
     * @brief Draw window.
     */
    auto draw() -> void
    {
        werase(handle_);                          // Clear Window
        wborder(handle_, 0, 0, 0, 0, 0, 0, 0, 0); // Draw Window border
        for (auto it = elements_.begin(); it != elements_.end(); ++it)
        {
            (*it)->draw(handle_); // Draw all Elements the window contains
        }
        wrefresh(handle_); // Refresh window. Draws all elements
    }

private:
    std::size_t                               x_;        // Window origin x-coordinate
    std::size_t                               y_;        // Window origin y-coordinate
    std::size_t                               w_;        // Window width
    std::size_t                               h_;        // Window height
    WINDOW *                                  handle_;   // Native curses window handle.
    std::vector<std::shared_ptr<BaseElement>> elements_; // Elements attached to this window
};

/**
 * @brief Create window object
 * @param[in] x          Window origin x-coordinate
 * @param[in] y          Window origin y-coordinate
 * @param[in] width      Window width
 * @param[in] height     Window height
 * @param[in] elements   The elements this window contains
 */
auto makeWindow(std::size_t                                       x,
                std::size_t                                       y,
                std::size_t                                       width,
                std::size_t                                       height,
                std::vector<std::shared_ptr<BaseElement>> const & elements
               ) -> std::shared_ptr<Window>
{
    return std::make_shared<Window>(x, y, width, height, elements);
}

#endif // WINDOW_HPP_201706140914
