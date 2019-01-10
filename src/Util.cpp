/**
 * @file      Util.cpp
 * @author    Simon Brummer (<simon.brummer@posteo.de>)
 * @brief     Utility functions.
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include "Util.hpp"

std::string_view trim_view(std::string_view const& s)
{
    auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c){return std::isspace(c);});
    auto wsback  = std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
    return (wsback <= wsfront) ? std::string_view() : std::string_view(wsfront, wsback - wsfront);
}

std::optional<int> string_to_int(std::string const& str)
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

void append_and_fill( std::string& dst, std::string const& src, unsigned len)
{
    auto left = static_cast<int>(len) - static_cast<int>(src.size());

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

void abort(std::string error_msg)
{
    std::cerr << "Error occured: '" << error_msg << "'. Abort" << std::endl;
    exit(-1);
}

std::string make_proto_port_string( std::string const&                protocol
                                  , std::optional<std::string> const& port
                                  )
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

std::string make_interval_string(std::string const& interval)
{
    return interval + "s";
}
