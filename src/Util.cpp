/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   Util.cpp
 * @author Simon Brummer
 * @date   08.04.2018
 */
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include "Util.hpp"

auto trim_view(std::string_view const& s) -> std::string_view
{
    auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c){return std::isspace(c);});
    auto wsback  = std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
    return (wsback <= wsfront) ? std::string_view() : std::string_view(wsfront, wsback - wsfront);
}

auto string_to_int(std::string const& str) -> std::optional<std::int32_t>
{
    try
    {
        return std::stoi(str);
    }
    catch (...)
    {
        return {};
    }
}

void append_and_fill( std::string& dst, std::string const& src, std::size_t len)
{
    ssize_t left = static_cast<ssize_t>(len)
                 - static_cast<ssize_t>(src.size());

    // src <= len -> Append to string and fill with ' ' up to len
    if (left >= 0)
    {
        dst.append(src);
        dst.append(left, ' ');
    }
    // src > len -> Append up to len from str
    else
    {
        dst.append(src.data(), len);
    }
}

auto abort(std::string error_msg) -> void
{
    std::cerr << "Error occured: '" << error_msg << "'. Abort" << std::endl;
    exit(-1);
}

auto make_proto_port_string( std::string const&                protocol
                           , std::optional<std::string> const& port) -> std::string
{
    auto tmp = protocol;
    if (port)
    {
        auto const& p = port.value();

        tmp.insert(tmp.end(), 1, ':');
        tmp.insert(tmp.end(), p.begin(), p.end());
    }
    return tmp;
}

auto make_interval_string(std::string const& interval) -> std::string
{
    return interval + "s";
}
