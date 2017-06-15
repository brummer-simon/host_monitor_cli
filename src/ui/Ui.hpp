/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   Ui.hpp
 * @author Simon Brummer
 * @date   14.6.2017
 */

#ifndef UI_HPP_201706140914
#define UI_HPP_201706140914

#include "UiConfig.hpp"
#include "Window.hpp"
#include "elements/Elements.hpp"

/**
 * @brief Textual user interface.
 */
class Ui
{
public:
    Ui(std::shared_ptr<Window> const & window)
        : window_(window)
    {
    }

    ~Ui()
    {
    }

    /**
     * @brief Draw user interfaces current state.
     */
    auto draw() -> void
    {
        window_->draw();
    }

private:
    std::shared_ptr<Window> window_; // The window the user interface consists of.
};

/**
 * @brief User interface generation function. This function handles all the element
 *        positioning and user interface generation.
 * @param[in] monitors     HostMonitor objects that should be listed by this ui.
 * @param[in] observers    HostMonitorObservers associated with @p monitors.
 * @param[in] termWidth    Width of the terminal the ui is placed into.
 * @param[in] termHeight   Height of the terminal the ui is placed into.
 * @returns   Pointer to draw-able user interface.
 */
auto makeUi(std::vector<std::shared_ptr<HostMonitor>> const&         monitors,
            std::vector<std::shared_ptr<HostMonitorObserver>> const& observers,
            std::size_t                                              termWidth,
            std::size_t                                              termHeight
           ) -> std::shared_ptr<Ui>
{
    // Generate main window content based on position
    size_t lenTarget = 0;
    size_t lenAlias  = 0;

    // Longest alias and target (needed for positioning)
    for (auto it = monitors.begin(); it != monitors.end(); ++it)
    {
        // Find longest alias length
        size_t len = 0;
        len = (*it)->getAlias().size();
        lenAlias = (len > lenAlias) ? len : lenAlias;

        // Find longest target length
        len = (*it)->getEndpoint().getTarget().size();
        lenTarget = (len > lenTarget) ? len : lenTarget;
    }

    // Ensure that UiLabels fit into lenAlias and lenTarget
    lenAlias  = (lenAlias < sizeof("Host")) ? sizeof("Host") : lenAlias;
    lenTarget = (lenTarget < sizeof("Hostname")) ? sizeof("Hostname") : lenTarget;

    // Calculate main window height and width
    size_t mainw = 0;
    mainw += lenAlias + lenTarget + UiConfig::lenProto + UiConfig::lenAvail;
    mainw += (UiConfig::border * 2) + (UiConfig::advance * 3) - 1;

    size_t mainh = monitors.size() + (UiConfig::border * 2) + 2;

    // Shrink main window in case it exceeds terminal
    mainw = (termWidth < mainw) ? termWidth : mainw;
    mainh = (termHeight < mainh) ? termHeight : mainh;

    // Calculate window origin
    size_t mainx = (termWidth >> 1) - (mainw >> 1);
    size_t mainy = (termHeight >> 1) - (mainh >> 1);

    // Fill Window with Elements
    size_t posX = 0;
    size_t posY = 0;
    std::vector<std::shared_ptr<BaseElement>> elements;

    // Create Headline
    posX = 0 + UiConfig::border;
    posY += 1;
    elements.push_back(makeTextElement(posX, posY, "Host"));
    posX += lenAlias + UiConfig::advance;
    elements.push_back(makeTextElement(posX, posY, "Hostname"));
    posX += lenTarget + UiConfig::advance;
    elements.push_back(makeTextElement(posX, posY, "Protocol"));
    posX += UiConfig::lenProto + UiConfig::advance;
    elements.push_back(makeTextElement(posX, posY, "Status"));

    // Create separator headline / main
    posX = 0 + UiConfig::border;
    posY += 1;
    elements.push_back(makeLineElement(posX, posY, LineElement::Orientation::Horizontal,
                                       mainw - (UiConfig::border * 2)));

    // Create Content
    for (size_t i = 0; i < monitors.size(); ++i)
    {
        HostMonitor& mon = *(monitors[i]);

        // Set position on next line
        posX = 0 + UiConfig::border;
        posY += 1;

        // Add Alias to line, advance by lenAlias
        elements.push_back(makeTextElement(posX, posY, mon.getAlias()));
        posX += lenAlias + UiConfig::advance;

        // Add Target to line, advance by lenAlias
        elements.push_back(makeTextElement(posX, posY, mon.getEndpoint().getTarget()));
        posX += lenTarget + UiConfig::advance;

        // Add Protocol to line
        switch (mon.getEndpoint().getProtocol())
        {
        case Protocol::ICMP:
            elements.push_back(makeTextElement(posX, posY, "ICMP"));
            break;

        case Protocol::TCP:
            elements.push_back(makeTextElement(posX, posY, "TCP"));
            break;
        }
        posX += UiConfig::lenProto + UiConfig::advance;

        // Add Observer to line
        elements.push_back(makeObserverElement(posX, posY, observers[i]));
    }

    // Return created user interface
    return std::shared_ptr<Ui>(new Ui(makeWindow(mainx, mainy, mainw, mainh, elements)));
}

#endif // UI_HPP_201706140914
