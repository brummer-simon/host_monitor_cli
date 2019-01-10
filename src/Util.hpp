/**
 * @file      Util.hpp
 * @author    Simon Brummer (<simon.brummer@posteo.de>)
 * @brief     Utility functions.
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef UTIL_HPP_201804081223
#define UTIL_HPP_201804081223

#include <string>
#include <optional>

// Remove leading and trailing whitespaces from given string_view
std::string_view trim_view(std::string_view const& s);

// Convert string to int. In case string can't be converted
// an empty optional is returned.
std::optional<int> string_to_int(std::string const& str);

// Append @p len characters from @p src to @p dst. In case len is more than src.size()
// the remaining characters are filled with spaces.
void append_and_fill( std::string& dst, std::string const& src, unsigned len);

// Abort program (critical error occured).
void abort(std::string error_msg);

// Make combined string with <protocol:port>.
std::string make_proto_port_string( std::string const&                protocol
                                  , std::optional<std::string> const& port
                                  );

// Make interval string <interval>s.
std::string make_interval_string(std::string const& interval);

#endif // UTIL_HPP_201804081223
