/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   UserInterface.hpp
 * @author Simon Brummer
 * @date   08.04.2018
 */

#ifndef USERINTERFACE_HPP_201804081223
#define USERINTERFACE_HPP_201804081223

#include <vector>
#include <string>
#include "Window.hpp"
#include "GroupElement.hpp"

// Ui class
class UserInterface
{
public:
    // Constructor. @p groups are the groups that should be in the ui.
    // @p fmt contains the fields on display and their order.
    UserInterface( std::vector<GroupElement::Pointer> const&  groups
                 , std::vector<ConfigGlobal::FieldFmt> const& fmt);

    ~UserInterface();

    // Rebuild entire ui. Call in case the terminal dimensions change.
    void rebuild_ui(void);

    // Draw current ui state.
    void draw(void);

    // Disable Copy and Move Semantics
    UserInterface(UserInterface const& other) = delete;
    UserInterface(UserInterface&& other) = delete;
    UserInterface& operator = (UserInterface const& other) = delete;
    UserInterface& operator = (UserInterface&& other) = delete;

private:
    // Setup curses
    void setup_curses();

    // Cleanup curses
    void teardown_curses();

    Window::Pointer                    wnd_;
    std::vector<GroupElement::Pointer> groups_;
    std::string                        header_;
    std::string                        footer_;
};

#endif // USERINTERFACE_HPP_201804081223