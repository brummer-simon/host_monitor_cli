/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   Config.hpp
 * @author Simon Brummer
 * @date   08.04.2018
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

auto field_to_string(Field field) -> std::string;
auto string_to_field(std::string_view const& str) -> Field;

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
    using FieldLen = size_t;
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
auto read_config_file(std::string const& path) -> Config;

// Config struct streaming functions
auto operator << (std::ostream& ost, Config& cfg) -> std::ostream&;
auto operator << (std::ostream& ost, ConfigGlobal& cfg) -> std::ostream&;
auto operator << (std::ostream& ost, ConfigGroup& cfg) -> std::ostream&;
auto operator << (std::ostream& ost, ConfigHost& cfg) -> std::ostream&;

#endif // CONFIG_HPP_201804081223
