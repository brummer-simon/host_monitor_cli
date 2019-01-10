/**
 * @file      Window.cpp
 * @author    Simon Brummer (<simon.brummer@posteo.de>)
 * @brief     Wrapper for ncurses windows
 * @copyright 2018 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "Window.hpp"

Window::Window( Position const& origin
              , unsigned        width
              , unsigned        height
              )
    : wnd_(nullptr)
    , origin_(origin)
    , width_(width)
    , height_(height)
    , color_(ColorPair( Window::Color::Default
                      , Window::Color::Default)
                      )
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
    static auto color_counter = short(0);

    color_ = ColorPair(fg, bg);

    // Lookup requested color pair. Create a new one if it cant be found.
    if (colors_.find(color_) == colors_.cend())
    {
        ++color_counter;
        colors_[color_] = color_counter;
    }

    // Setup curses color pair and enable color.
    auto color_id = colors_[color_];
    std::tie(fg, bg) = color_;

    init_pair(color_id
             , static_cast<short>(fg)
             , static_cast<short>(bg)
             );

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

void Window::set_underlined()
{
    wattron(wnd_.get(), A_UNDERLINE);
}

void Window::unset_underlined()
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

void Window::add_vertical_line(unsigned len)
{
    add_vertical_line(0, len);
}

void Window::add_vertical_line(char ch, unsigned len)
{
    wvline(wnd_.get(), ch, len);
}

void Window::add_horizontal_line(unsigned len)
{
    add_horizontal_line(0, len);
}

void Window::add_horizontal_line(char ch, unsigned len)
{
    whline(wnd_.get(), ch, len);
}

void Window::add_border(void)
{
    add_border(0, 0, 0, 0, 0, 0, 0, 0);
}

void Window::add_border( char left_side
                       , char right_side
                       , char top_side
                       , char bottom_side
                       , char top_left_corner
                       , char top_right_corner
                       , char bottom_left_corner
                       , char bottom_right_corner
                       )
{
    wborder( wnd_.get()
           , left_side
           , right_side
           , top_side
           , bottom_side
           , top_left_corner
           , top_right_corner
           , bottom_left_corner
           , bottom_right_corner
           );
}

void Window::erase()
{
    werase(wnd_.get());
}

void Window::refresh()
{
    wrefresh(wnd_.get());
}

Position Window::get_origin() const
{
    return origin_;
}

unsigned Window::get_width() const
{
    return width_;
}

unsigned Window::get_height() const
{
    return height_;
}

Window::Color Window::get_fg_color() const
{
    return color_.first;
}

Window::Color Window::get_bg_color() const
{
    return color_.second;
}

Window::CursesWnd* Window::get_raw_pointer() const
{
    return wnd_.get();
}
