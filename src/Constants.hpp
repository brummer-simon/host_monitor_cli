/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   Constants.hpp
 * @author Simon Brummer
 * @date   8.4.2018
 */

#ifndef CONSTANTS_HPP_201804081223
#define CONSTANTS_HPP_201804081223

#include <cstdint>

// Config Parser Constants
char const         cfg_delimiter                   = ' ';
char const         cfg_comment_marker              = '#';
char const * const cfg_default_config_file         = ".host_monitor_cli";
char const * const cfg_marker_config_section_begin = "BEGIN_CONFIG";
char const * const cfg_marker_config_section_end   = "END_CONFIG";
char const * const cfg_marker_config_field_order   = "FIELD_ORDER:";
char const * const cfg_marker_group_section_begin  = "BEGIN_GROUP";
char const * const cfg_marker_group_section_end    = "END_GROUP";
char const * const cfg_marker_group_name           = "NAME:";
char const * const cfg_marker_host_section_begin   = "BEGIN_HOST";
char const * const cfg_marker_host_section_end     = "END_HOST";
char const * const cfg_marker_host_fqhn            = "FQHN:";
char const * const cfg_marker_host_alias           = "ALIAS:";
char const * const cfg_marker_host_role            = "ROLE:";
char const * const cfg_marker_host_device          = "DEVICE:";
char const * const cfg_marker_host_protocol        = "PROTOCOL:";
char const * const cfg_marker_host_port            = "PORT:";
char const * const cfg_marker_host_interval        = "INTERVAL:";

// UI Constatants
std::uint16_t const ui_border_width         = 1;
std::uint16_t const ui_border_gap           = 1;
char const          ui_field_space[]        = "   ";
char const          ui_status_available[]   = "available";
char const          ui_status_unavailable[] = "unavailable";
std::uint16_t const ui_line_offset_x        = 1;
std::uint16_t const ui_line_offset_y        = 1;
std::uint16_t const ui_header_height        = 2;
char const          ui_header_fqhn[]        = "FHQN:";
char const          ui_header_alias[]       = "Alias:";
char const          ui_header_role[]        = "Role:";
char const          ui_header_device[]      = "Device:";
char const          ui_header_protocol[]    = "Protocol:";
char const          ui_header_interval[]    = "Interval:";
char const          ui_header_status[]      = "Status:";
std::uint16_t const ui_footer_height        = 2;
char const          ui_footer_quit[]        = "Press 'ctrl + c' to quit.";
std::uint16_t const ui_observer_elem_height = 1;

#endif // CONSTANTS_HPP_201804081223