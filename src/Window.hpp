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
 * @date   08.04.2018
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
    std::int32_t x = 0;
    std::int32_t y = 0;

    Position()
    {
    }

    Position( std::int32_t x
            , std::int32_t y)
        : x(x)
        , y(y)
    {
    }
};

// Curses wrapping window class.
class Window
{
public:
    using Pointer = std::shared_ptr<Window>;
    using CursesWnd = WINDOW;

    // Predefined colors by curses
    enum class Color
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
          , std::uint32_t   width
          , std::uint32_t   height);

    // Move to specific position in window, relative to the windows origin.
    void move_to(Position const& pos);

    // Set current foreground color.
    void set_foreground_color(Window::Color c);
    
    // Set current background color.
    void set_background_color(Window::Color c);

    // Set fore- and background color.
    void set_color( Window::Color fg
                  , Window::Color bg);

    // Revert setted color back to the default.
    // Before setting a new color, unset has to be called.
    void unset_color(void);

    // Add underlined to current attributes.
    void set_underlined(void);

    // Remove underlined from current attributes.
    void unset_underlined(void);

    // Add string to current position.
    template<typename... Args>
    void add_string(char const *fmt, Args... args)
    {
        wprintw(wnd_.get(), fmt, args...);
    }

    // Add string to current position.
    void add_string(std::string const& str);

    // Add string with up to @p str_len characters to current position.
    void add_string( std::string const& str
                   , std::size_t        str_len);

    // Add c-string with up to @p str_len characters to current position.
    void add_string(char const *str, std::size_t str_len);

    // Add vertical line to current position with default character.
    void add_vertical_line(std::uint32_t len);

    // Add vertical line to current position with user defined character.
    void add_vertical_line(std::uint8_t ch, std::uint32_t len);

    // Add horizontal line to current position with default character.
    void add_horizontal_line(std::uint32_t len);

    // Add horizontal line to current position with user defined character.
    void add_horizontal_line(std::uint8_t ch, std::uint32_t len);

    // Add default border around entire window.
    void add_border(void);

    // Add user defined border around entire window.
    void add_border( std::uint8_t left_side
                   , std::uint8_t right_side
                   , std::uint8_t top_side
                   , std::uint8_t bottom_side
                   , std::uint8_t top_left_corner
                   , std::uint8_t top_right_corner
                   , std::uint8_t bottom_left_corner
                   , std::uint8_t bottom_right_corner);

    // Clear entire windows contents
    void erase(void);

    // Show current window state. Changes are only visible after calling refresh.
    void refresh(void);

    // Get window origin.
    auto get_origin(void) -> Position;

    // Get window width.
    auto get_width(void) -> std::uint32_t;

    // Get window height.
    auto get_height(void) -> std::uint32_t;

    // Get current foreground color.
    auto get_fg_color(void) -> Window::Color;

    // Get current background color.
    auto get_bg_color(void) -> Window::Color;

    // Get raw pointer to curses window.
    auto get_raw_pointer(void) -> CursesWnd *;

private:
    using CursesWndDel = std::function<void(CursesWnd*)>;
    using ColorPair = std::pair<Window::Color, Window::Color>;
    using ColorMap = std::map<ColorPair, std::uint16_t>;

    // Handle on curses window with custom deleter for automatic cleanup.
    std::unique_ptr<CursesWnd, CursesWndDel> wnd_;

    Position      origin_;
    std::uint32_t width_;
    std::uint32_t height_;
    ColorPair     color_;
    ColorMap      colors_;
};

#endif // WINDOW_HPP_201804081223