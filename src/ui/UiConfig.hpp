/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   UiConfig.hpp
 * @author Simon Brummer
 * @date   14.6.2017
 */

#ifndef UICONFIG_HPP_201706140914
#define UICONFIG_HPP_201706140914

/**
 * @brief User interface configuration.
 */
struct UiConfig
{
    static constexpr char const * const avail    = "available";
    static constexpr char const * const nonAvail = "non-available";
    static size_t const                 lenAvail = sizeof("non-available");

    static size_t const advance = 3;  // Advance Distance between fields
    static size_t const lenProto = 8; // Max length of field protocol
    static size_t const border = 1;   // Border thickness
};

#endif // UICONFIG_HPP_201706140914
