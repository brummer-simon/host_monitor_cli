/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   Util.hpp
 * @author Simon Brummer
 * @date   08.04.2018
 */

#ifndef UTIL_HPP_201804081223
#define UTIL_HPP_201804081223

#include <string>
#include <optional>

// Remove leading and trailing whitespaces from given string_view
auto trim_view(std::string_view const& s) -> std::string_view;

// Convert string to int. In case string can't be converted
// an empty optional is returned.
auto string_to_int(std::string const& str) -> std::optional<std::int32_t>;

// Append @p len characters from @p src to @p dst. In case len is more than src.size()
// the remaining characters are filled with spaces.
void append_and_fill( std::string& dst, std::string const& src, std::size_t len);

// Abort program (critical error occured).
void abort(std::string error_msg);

// Make combined string with <protocol:port>.
auto make_proto_port_string( std::string const&                protocol
                           , std::optional<std::string> const& port) -> std::string;

// Make interval string <interval>s.
auto make_interval_string(std::string const& interval) -> std::string;

#endif // UTIL_HPP_201804081223
