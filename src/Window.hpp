/**
 * @file      Window.hpp
 * @author    Simon Brummer (<simon.brummer@posteo.de>)
 * @brief     Wrapper for ncurses windows
 * @copyright 2018 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef WINDOW_HPP_201804081223
#define WINDOW_HPP_201804081223

#include <memory>
#include <map>
#include <functional>
#include <cstdint>
#include <curses.h>

// Helper struct. Position in x,y - plane
struct Position
{
    unsigned x = 0;
    unsigned y = 0;

    Position() = default;
    Position( unsigned x
            , unsigned y
            )
        : x(x)
        , y(y)
    {
    }
};

// Curses wrapping window class.
class Window
{
public:
    using Pointer   = std::shared_ptr<Window>;
    using CursesWnd = WINDOW;

    // Predefined colors by curses
    enum class Color : short
    {
        Default = -1,
        Black   = COLOR_BLACK,
        Red     = COLOR_RED,
        Green   = COLOR_GREEN,
        Yellow  = COLOR_YELLOW,
        Blue    = COLOR_BLUE,
        Magenta = COLOR_MAGENTA,
        Cyan    = COLOR_CYAN,
        White   = COLOR_WHITE
    };

    // Constructor: @p origin is the position of upper left corner of window
    // @p width and @p height are the windows height and width in terms of characters.
    Window( Position const& origin
          , unsigned        width
          , unsigned        height
          );

    // Move to specific position in window, relative to the windows origin.
    void move_to(Position const& pos);

    // Set current foreground color.
    void set_foreground_color(Window::Color c);

    // Set current background color.
    void set_background_color(Window::Color c);

    // Set fore- and background color.
    void set_color(Window::Color fg, Window::Color bg);

    // Revert current color back to the default.
    // Before setting a new color, unset has to be called.
    void unset_color();

    // Add underlined to current attributes.
    void set_underlined();

    // Remove underlined from current attributes.
    void unset_underlined();

    // Add string to current position.
    template<typename... Args>
    void add_string(char const *fmt, Args... args)
    {
        wprintw(wnd_.get(), fmt, args...);
    }

    // Add string to current position.
    void add_string(std::string const& str);

    // Add string with up to @p str_len characters to current position.
    void add_string( std::string const& str, std::size_t str_len);

    // Add c-string with up to @p str_len characters to current position.
    void add_string(char const *str, std::size_t str_len);

    // Add vertical line to current position with default character.
    void add_vertical_line(unsigned len);

    // Add vertical line to current position with user defined character.
    void add_vertical_line(char ch, unsigned len);

    // Add horizontal line to current position with default character.
    void add_horizontal_line(unsigned len);

    // Add horizontal line to current position with user defined character.
    void add_horizontal_line(char ch, unsigned len);

    // Add default border around entire window.
    void add_border(void);

    // Add user defined border around entire window.
    void add_border( char left_side
                   , char right_side
                   , char top_side
                   , char bottom_side
                   , char top_left_corner
                   , char top_right_corner
                   , char bottom_left_corner
                   , char bottom_right_corner
                   );

    // Clear entire windows contents
    void erase();

    // Show current window state. Changes are only visible after calling refresh.
    void refresh();

    // Get window origin.
    Position get_origin() const;

    // Get window width.
    unsigned get_width() const;

    // Get window height.
    unsigned get_height() const;

    // Get current foreground color.
    Window::Color get_fg_color() const;

    // Get current background color.
    Window::Color get_bg_color() const;

    // Get raw pointer to curses window.
    CursesWnd* get_raw_pointer() const;

private:
    using CursesWndDel = std::function<void(CursesWnd*)>;
    using ColorPair    = std::pair<Window::Color, Window::Color>;
    using ColorMap     = std::map<ColorPair, short>;

    // Handle on curses window with custom deleter for automatic cleanup.
    std::unique_ptr<CursesWnd, CursesWndDel> wnd_;

    Position  origin_;
    unsigned  width_;
    unsigned  height_;
    ColorPair color_;
    ColorMap  colors_;
};

#endif // WINDOW_HPP_201804081223
