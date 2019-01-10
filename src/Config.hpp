/**
 * @file      Config.hpp
 * @author    Simon Brummer (<simon.brummer@posteo.de>)
 * @brief     Command line argument parsing.
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef CONFIG_HPP_201804081223
#define CONFIG_HPP_201804081223

#include <vector>
#include <string>
#include <optional>
#include <iostream>
#include <cstdint>

// UI Field enum and some conversion functions.
enum class Field
{
    Undef = 0,
    Fqhn,
    Alias,
    Role,
    Device,
    Protocol,
    Interval
};

std::string field_to_string(Field field);
Field string_to_field(std::string_view const& str);

// Configuration Objects
struct ConfigHost
{
    std::string                fqhn;
    std::string                protocol;
    std::string                interval;
    std::optional<std::string> alias;
    std::optional<std::string> role;
    std::optional<std::string> device;
    std::optional<std::string> port;
};

struct ConfigGroup
{
    std::optional<std::string> name;
    std::vector<ConfigHost>    hosts;
};

struct ConfigGlobal
{
    using FieldLen = unsigned;
    using FieldFmt = std::pair<Field, FieldLen>;

    std::string           field_order;
    std::vector<FieldFmt> field_format;
};

struct Config
{
    ConfigGlobal             global;
    std::vector<ConfigGroup> groups;
};

// Read configuration from file.
Config read_config_file(std::string const& path);

std::ostream& operator << (std::ostream& ost, Config const& cfg);
std::ostream& operator << (std::ostream& ost, ConfigGlobal const& cfg);
std::ostream& operator << (std::ostream& ost, ConfigGroup const& cfg);
std::ostream& operator << (std::ostream& ost, ConfigHost const& cfg);

#endif // CONFIG_HPP_201804081223
