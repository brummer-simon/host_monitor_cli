/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   Terminal.hpp
 * @author Simon Brummer
 * @date   14.6.2017
 */

#ifndef TERMINAL_HPP_201706140914
#define TERMINAL_HPP_201706140914

#include <curses.h>

/**
 * @brief Terminal setup class. Wrapper for ncurses screen initialization.
 */
class Terminal
{
public:
    Terminal()
    {
        setupTerm();
        refreshTermDimensions();
    }

    ~Terminal()
    {
        cleanupTerm();
    }

    auto refreshTermDimensions() -> void
    {
        getmaxyx(stdscr, this->height_, this->width_);
    }

    auto reset() -> void
    {
        cleanupTerm();
        setupTerm();
        refreshTermDimensions();
    }

    auto getWidth() -> std::size_t
    {
        return this->width_;
    }

    auto getHeight() -> std::size_t
    {
        return this->height_;
    }

private:
    auto setupTerm() -> void
    {
        initscr();    // Init ncurses
        noecho();     // Suppress typed characters
        curs_set(0);  // Hide cursor
        clear();      // Clear entire screen
        refresh();    // Refresh screen with empty screen
    }
    auto cleanupTerm() -> void
    {
        endwin();   // Restore terminal settings
    }

    std::size_t width_;  // Current terminal width
    std::size_t height_; // Current terminal height
};

#endif // TERMINAL_HPP_201706140914
