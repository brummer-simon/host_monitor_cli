/**
 * @file      UserInterface.cpp
 * @author    Simon Brummer (<simon.brummer@posteo.de>)
 * @brief     Command line argument parsing.
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <cstdint>
#include <cstring>
#include "Constants.hpp"
#include "Util.hpp"
#include "UserInterface.hpp"

namespace
{
// Create header string from given field format.
std::string make_header_string(std::vector<ConfigGlobal::FieldFmt> const& fmt)
{
    auto tmp = std::string();

    for (auto const& [type, len] : fmt)
    {
        switch (type)
        {
            case Field::Fqhn:
                append_and_fill(tmp, ui_header_fqhn, len);
                break;

            case Field::Alias:
                append_and_fill(tmp, ui_header_alias, len);
                break;

            case Field::Role:
                append_and_fill(tmp, ui_header_role, len);
                break;

            case Field::Device:
                append_and_fill(tmp, ui_header_device, len);
                break;

            case Field::Protocol:
                append_and_fill(tmp, ui_header_protocol, len);
                break;

            case Field::Interval:
                append_and_fill(tmp, ui_header_interval, len);
                break;

            default:
                append_and_fill(tmp, "", len);
        }

        // Append spacer between fields
        tmp.append(ui_field_space);
    }

    // Append status
    append_and_fill(tmp, ui_header_status, std::max( std::strlen(ui_status_available)
                                                   , std::strlen(ui_status_unavailable))
                                                   );

    return tmp;
}
} // namespace anon

UserInterface::UserInterface( std::vector<GroupElement::Pointer> const&  groups
                            , std::vector<ConfigGlobal::FieldFmt> const& fmt
                            )
    : wnd_(nullptr)
    , groups_(groups)
{
    header_ = make_header_string(fmt);

    setup_curses();
}

UserInterface::~UserInterface()
{
    teardown_curses();
}

void UserInterface::rebuild_ui(void)
{
    teardown_curses();
    setup_curses();
}

void UserInterface::draw(void)
{
    auto line_len   = 0;
    auto chars_left = 0;

    // Calculate line length and chars per line. Prevent underflows.
    line_len = wnd_->get_width() - (2 * ui_border_width);
    line_len = (line_len < 0) ? 0 : line_len;

    chars_left = line_len - (2 * ui_line_offset_x);
    chars_left = (chars_left < 0) ? 0 : chars_left;

    auto pos = Position{ ui_border_width + ui_line_offset_x
                       , ui_border_width };

    // Prepare Window
    wnd_->erase();

    // Add Header
    wnd_->move_to(pos);
    wnd_->add_string(header_, chars_left);

    pos.x = ui_border_width;
    pos.y += ui_line_offset_y;
    wnd_->move_to(pos);
    wnd_->add_horizontal_line(line_len);

    // Add Groups
    for (auto grp : groups_)
    {
        grp->draw(wnd_, pos);

        // Add empty line between groups
        pos.y += 1;
    }
    pos.y -= 1;

    // Add Footer
    pos.x = ui_border_width;
    pos.y += ui_line_offset_y;
    wnd_->move_to(pos);
    wnd_->add_horizontal_line(line_len);

    pos.x += ui_line_offset_x;
    pos.y += ui_line_offset_y;
    wnd_->move_to(pos);
    wnd_->add_string(ui_footer_quit, chars_left);

    // Refresh
    wnd_->add_border();
    wnd_->refresh();
}

void UserInterface::setup_curses()
{
    auto height           = unsigned(0);
    auto width            = unsigned(0);
    auto content_origin_x = unsigned(0);
    auto content_origin_y = unsigned(0);
    auto content_height   = unsigned(0);
    auto content_width    = unsigned(0);

    // Setup curses screen
    initscr();
    noecho();
    curs_set(0);
    start_color();
    use_default_colors();
    clear();
    refresh();

    // Setup Window
    getmaxyx(stdscr, height, width);

    // Assemble ui height and width from the contents
    for (auto grp : groups_)
    {
        content_height += grp->get_height();
        content_width = std::max(content_width, grp->get_width());
    }

    // Add lines for whitespaces between groups
    content_height += static_cast<unsigned>(ui_line_offset_y * groups_.size() - 1);

    // Add Room of the Header and Footer
    content_height += ui_header_height;
    content_width = std::max( content_width
                            , static_cast<unsigned>(header_.size())
                            );

    content_height += ui_footer_height;
    content_width = std::max( content_width
                            , static_cast<unsigned>(std::strlen(ui_footer_quit))
                            );

    // Add Borders
    content_width  += 2 * ui_border_width;
    content_height += 2 * ui_border_width;

    // Calculate window origin and prevent programm from
    // crashing if the window doesn't fit into the terminal.
    if (content_width < width)
    {
        content_origin_x = (width >> 1) - (content_width >> 1);
    }
    else
    {
        content_origin_x = 0;
        content_width = width;
    }

    if (content_height < height)
    {
        content_origin_y = (height >> 1) - (content_height >> 1);
    }
    else
    {
        content_origin_y = 0;
        content_height = height;
    }

    // Rebuild window with based on current terminal size
    wnd_ = std::make_shared<Window>( Position( content_origin_x
                                             , content_origin_y
                                             )
                                   , content_width
                                   , content_height
                                   );
}

void UserInterface::teardown_curses()
{
    // Cleanup Window
    wnd_->erase();
    wnd_->refresh();
    wnd_.reset();

    // Cleanup Curses
    clear();
    refresh();
    endwin();
}
