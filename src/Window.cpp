/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   Window.cpp
 * @author Simon Brummer
 * @date   08.04.2018
 */

#include "Window.hpp"

Window::Window( Position const& origin
              , std::uint32_t   width
              , std::uint32_t   height)
    : wnd_(nullptr)
    , origin_(origin)
    , width_(width)
    , height_(height)
    , color_(ColorPair( Window::Color::Default 
                      , Window::Color::Default))
{
    // Create Window Pointer
    auto pointer = newwin(height, width, origin.y, origin.x);
    auto deleter = [] (CursesWnd *p)
    {
        // Erase window content and refresh display.
        werase(p);
        wrefresh(p);

        // Free Resources
        delwin(p);
    };
    wnd_ = std::unique_ptr<CursesWnd, CursesWndDel>(pointer, deleter);
}

void Window::move_to(Position const& pos)
{
    wmove(wnd_.get(), pos.y, pos.x);
}

void Window::set_foreground_color(Window::Color c)
{
    set_color(c, get_bg_color());
}

void Window::set_background_color(Window::Color c)
{
    set_color(get_fg_color(), c);
}

void Window::set_color(Window::Color fg, Window::Color bg)
{
    static auto color_counter = std::size_t(0);

    color_ = ColorPair(fg, bg);

    // Lookup requested colorpair. Create a new one if it cant be found.
    if (colors_.find(color_) == colors_.cend())
    {
        color_counter  += 1;
        colors_[color_] = color_counter;
    }

    // Setup curses color pair and enable color.
    auto color_id = colors_[color_];
    std::tie(fg, bg) = color_;

    init_pair( color_id
             , static_cast<uint16_t>(fg)
             , static_cast<uint16_t>(bg));

    wattron(wnd_.get(), COLOR_PAIR(color_id));
}

void Window::unset_color()
{
    // Lookup color
    auto const pos = colors_.find(color_);
    if (pos == colors_.cend())
    {
        return;
    }

    wattroff(wnd_.get(), COLOR_PAIR(pos->second));
}

void Window::set_underlined(void)
{
    wattron(wnd_.get(), A_UNDERLINE);
}

void Window::unset_underlined(void)
{
    wattroff(wnd_.get(), A_UNDERLINE);
}

void Window::add_string(std::string const& str)
{
    add_string("%s", str.c_str());
}

void Window::add_string(std::string const& str, std::size_t str_len)
{
    if (str_len < str.size())
    {
        add_string(std::string(str, 0, str_len));
    }
    else
    {
        add_string(str);
    }
}

void Window::add_vertical_line(std::uint32_t len)
{
    add_vertical_line(0, len);
}

void Window::add_vertical_line(std::uint8_t ch, std::uint32_t len)
{
    wvline(wnd_.get(), ch, len);
}

void Window::add_horizontal_line(std::uint32_t len)
{
    add_horizontal_line(0, len);
}

void Window::add_horizontal_line(std::uint8_t ch, std::uint32_t len)
{
    whline(wnd_.get(), ch, len);
}

void Window::add_border(void)
{
    add_border(0, 0, 0, 0, 0, 0, 0, 0);
}

void Window::add_border( std::uint8_t left_side
                       , std::uint8_t right_side
                       , std::uint8_t top_side
                       , std::uint8_t bottom_side
                       , std::uint8_t top_left_corner
                       , std::uint8_t top_right_corner
                       , std::uint8_t bottom_left_corner
                       , std::uint8_t bottom_right_corner)
{
    wborder( wnd_.get()
           , left_side
           , right_side
           , top_side
           , bottom_side
           , top_left_corner
           , top_right_corner
           , bottom_left_corner
           , bottom_right_corner);
}

void Window::erase(void)
{
    werase(wnd_.get());
}

void Window::refresh(void)
{
    wrefresh(wnd_.get());
}

auto Window::get_origin(void) -> Position
{
    return origin_;
}

auto Window::get_width(void) -> std::uint32_t
{
    return width_;
}

auto Window::get_height(void) -> std::uint32_t
{
    return height_;
}

auto Window::get_fg_color(void) -> Window::Color
{
    return color_.first;
}

auto Window::get_bg_color(void) -> Window::Color
{
    return color_.second;
}

auto Window::get_raw_pointer(void) -> CursesWnd *
{
    return wnd_.get();
}