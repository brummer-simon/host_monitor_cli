/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   ObserverElement.hpp
 * @author Simon Brummer
 * @date   14.6.2017
 */

#ifndef OBSERVERELEMENT_HPP_201706141249
#define OBSERVERELEMENT_HPP_201706141249

#include <memory>
#include <cstdint>
#include <curses.h>
#include "BaseElement.hpp"

#include "../UiConfig.hpp"
#include "../../Observer.hpp"

/**
 * @brief Element getting its content from a HostMonitorObserver.
 */
class ObserverElement : public BaseElement
{
public:
    /**
     * @brief Fully specified constructor.
     * @param[in] originX    x-coordinate element origin.
     * @param[in] originY    y-coordinate element origin.
     * @param[in] observer   Pointer to the observer that holds the connection state.
     */
    ObserverElement(std::size_t originX, std::size_t originY, std::shared_ptr<Observer> observer)
        : BaseElement(originX, originY)
        , observer(observer)
    {
    }

    /**
     * @brief Destructor
     */
    virtual ~ObserverElement()
    {
    }

    /**
     * @brief Draw current state of observed host.
     * @param[in] wnd   Pointer to Window this Element should be drawn in.
     */
    virtual auto draw(WINDOW *wnd) -> void const
    {

        BaseElement::draw(wnd);
        if (observer->avail())
        {
            init_pair(1, COLOR_GREEN, COLOR_BLACK);
            wattron(wnd, COLOR_PAIR(1));
            wprintw(wnd, "%s", UiConfig::avail);
            wattroff(wnd, COLOR_PAIR(1));
        }
        else
        {
            init_pair(2, COLOR_RED, COLOR_BLACK);
            wattron(wnd, COLOR_PAIR(2));
            wprintw(wnd, "%s", UiConfig::nonAvail);
            wattroff(wnd, COLOR_PAIR(2));
        }
    }

protected:
    std::shared_ptr<Observer> observer; // Pointer to Observer holding the host_monitor_state.
};

/**
 * @brief Generator function for ObserverElements.
 * @param[in] x   x-coordinate element origin.
 * @param[in] y   y-coordinate element origin.
 * @param[in] o   Pointer to the observer that holds the connection state.
 * @returns   Shared pointer of Type BaseElement
 */
auto makeObserverElement(size_t x, size_t y, std::shared_ptr<HostMonitorObserver> o
                        ) ->std::shared_ptr<BaseElement>
{
    return std::make_shared<ObserverElement>(x, y, std::dynamic_pointer_cast<Observer>(o));
}


#endif // OBSERVERELEMENT_HPP_201706141249
