/**
 * @file      Config.cpp
 * @author    Simon Brummer (<simon.brummer@posteo.de>)
 * @brief     Command line argument parsing.
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <fstream>
#include <cstring>
#include <host_monitor/Endpoint.hpp>
#include "Util.hpp"
#include "Constants.hpp"
#include "Config.hpp"

using namespace host_monitor;

namespace
{
using LineNo      = unsigned;
using LineContent = std::string;
using Line        = std::pair<LineNo, LineContent>;

// Parsing error. Prints line the error occured
void abort_parsing(Line const& line, std::string error_msg)
{
    std::cerr << "Conifg parsing error: '" << error_msg << "' in";
    std::cerr << " line: '" << line.first;
    std::cerr << "',' expression: '" << line.second << "'. Abort";
    std::cerr << std::endl;
    exit(-1);
}

// Get content from line
std::string_view get_line_content(Line const& line)
{
    return std::string_view(line.second);
}

// Get marker from line
std::string_view get_line_marker(Line const& line)
{
    // Convert line to view an trim it
    auto content = get_line_content(line);
    content = trim_view(content);

    // Search for delimiter in content and "remove" anything from there
    auto pos = content.find(cfg_delimiter);
    if (pos != content.npos)
    {
        content.remove_suffix(content.size() - pos);
    }

    // If nothing is left: This is an error!
    content = trim_view(content);
    if (content.empty())
    {
        abort_parsing(line, "Keyword expected");
    }
    return content;
}

// Get value from a line associated with a marker
std::string_view get_line_value(Line const& line, char const *marker)
{
    // Convert line to view an trim it
    auto content = get_line_content(line);
    content = trim_view(content);

    // Discard marker from view
    auto pos = content.find(marker);
    if (pos == content.npos)
    {
        // In case no delimiter was found: No marker was found. This is an error.
        abort_parsing(line, "Given marker doesn't occur in given line.");
    }
    else
    {
        // Delimiter was found. Remove anything before it.
        content.remove_prefix(pos + std::strlen(marker));
    }

    // If nothing is left: This is an error!
    content = trim_view(content);
    if (content.empty())
    {
        abort_parsing(line, "Line contains no value");
    }
    return content;
}

// Remove comments and trim the result
std::string_view trim_and_remove_comment(std::string const& line)
{
    auto line_view = std::string_view(line);

    // Remove Comments from view
    auto pos_delimiter = line_view.find(cfg_comment_marker);
    if (pos_delimiter != line_view.npos)
    {
        line_view = line_view.substr(0, pos_delimiter);
    }

    // Trim view an return it
    return trim_view(line_view);
}

// Load given config file into a vector. Discards empty lines and comments.
void load_config_file( std::string const& cfg_file_path
                     , std::vector<Line>& cfg_file_content
                     )
{
    // Try to open config file.
    std::ifstream ifs(cfg_file_path.c_str(), std::ifstream::in);
    if (!ifs.is_open())
    {
        auto msg = std::string("Can't open config file '") + cfg_file_path + "'";
        abort(msg);
    }

    auto line_no = LineNo(0);

    // Read configuration file into vector line by line.
    while(ifs)
    {
        line_no += 1;

        auto line = LineContent();
        std::getline(ifs, line);

        // Discard lines that are empty or contain only comments.
        auto line_view = trim_and_remove_comment(line);
        if (line_view.empty())
        {
            continue;
        }

        // In case the line contains any content:
        // Construct string from view. This line i needed later
        auto lineContent = std::string( line_view.begin(), line_view.end());
        cfg_file_content.push_back(Line(line_no, std::move(lineContent)));
    }
    ifs.close();
}

// Read host section into config structure
void read_host_section( std::vector<Line>::const_iterator& line
                      , std::vector<Line>::const_iterator  end
                      , ConfigGroup&                       grp
                      )
{
    auto section = ConfigHost();
    auto begin = line;
    auto marker = get_line_marker(*begin);

    // Check that section starts with start marker
    if (marker != cfg_marker_host_section_begin)
    {
        auto msg = std::string("Expected '") + cfg_marker_host_section_begin + "'";
        abort_parsing(*begin, msg);
    }

    // Given iterator seems legit, continue reading
    ++line;
    for (; line != end; ++line)
    {
        // Valid marker are:
        // 1) Fully qualified host name (FQHN)
        // 2) Alias
        // 3) Role
        // 4) Device
        // 5) Protocol
        // 6) Port
        // 7) Interval
        // 8) End of host
        marker = get_line_marker(*line);

        // 1) Read FQHN
        if (marker == cfg_marker_host_fqhn)
        {
            auto view = get_line_value(*line, cfg_marker_host_fqhn);
            section.fqhn = std::move(std::string(view.begin(), view.end()));
        }
        // 2) Read Alias
        else if (marker == cfg_marker_host_alias)
        {
            auto view = get_line_value(*line, cfg_marker_host_alias);
            section.alias = std::move(std::string(view.begin(), view.end()));
        }
        // 3) Read Role
        else if (marker == cfg_marker_host_role)
        {
            auto view = get_line_value(*line, cfg_marker_host_role);
            section.role = std::move(std::string(view.begin(), view.end()));
        }
        // 4) Read Device
        else if (marker == cfg_marker_host_device)
        {
            auto view = get_line_value(*line, cfg_marker_host_device);
            section.device = std::move(std::string(view.begin(), view.end()));
        }
        // 5) Read Protocol
        else if (marker == cfg_marker_host_protocol)
        {
            auto view = get_line_value(*line, cfg_marker_host_protocol);
            section.protocol = std::move(std::string(view.begin(), view.end()));
        }
        // 6) Read Port
        else if (marker == cfg_marker_host_port)
        {
            auto view = get_line_value(*line, cfg_marker_host_port);
            section.port = std::move(std::string(view.begin(), view.end()));
        }
        // 7) Read Interval
        else if (marker == cfg_marker_host_interval)
        {
            auto view = get_line_value(*line, cfg_marker_host_interval);
            section.interval = std::move(std::string(view.begin(), view.end()));
        }
        // 8) Read section end. Assign read section and return.
        else if(marker == cfg_marker_host_section_end)
        {
            grp.hosts.push_back(std::move(section));
            return;
        }
        // Anything else is an error
        else
        {
            abort_parsing(*line, "Unexpected config entry");
        }
    }

    // In case we end up here: This is an error, the section was not closed. Abort.
    abort_parsing(*begin, "Host section was never closed");
}

// Read group section into config structure
void read_group_section( std::vector<Line>::const_iterator& line
                       , std::vector<Line>::const_iterator  end
                       , Config&                            cfg
                       )
{
    auto section = ConfigGroup();
    auto begin = line;
    auto marker = get_line_marker(*begin);

    // Check that section starts with start marker
    if (marker != cfg_marker_group_section_begin)
    {
        auto msg = std::string("Expected '") + cfg_marker_group_section_begin + "'";
        abort_parsing(*begin, msg);
    }

    // Given iterator seems legit, continue reading
    ++line;
    for (; line != end; ++line)
    {
        // Valid marker are:
        // 1) Groupname
        // 2) Begin of host
        // 3) End of group
        marker = get_line_marker(*line);

        // 1) Read groupname
        if (marker == cfg_marker_group_name)
        {
            auto view = get_line_value(*line, cfg_marker_group_name);
            section.name = std::move(std::string(view.begin(), view.end()));
        }
        // 2) Read host section.
        else if(marker == cfg_marker_host_section_begin)
        {
            read_host_section(line, end, section);
        }
        // 3) Read section end. Assign read section and return.
        else if(marker == cfg_marker_group_section_end)
        {
            cfg.groups.push_back(std::move(section));
            return;
        }
        // Anything else is an error
        else
        {
            abort_parsing(*line, "Unexpected config entry");
        }
    }

    // In case we end up here: This is an error, the section was not closed. Abort.
    abort_parsing(*begin, "Group section was never closed");
}

// Read config section into config structure
void read_config_section( std::vector<Line>::const_iterator& line
                        , std::vector<Line>::const_iterator  end
                        , Config&                            cfg
                        )
{
    auto section = ConfigGlobal();
    auto begin = line;
    auto marker = get_line_marker(*begin);

    // Check that section starts with start marker
    if (marker != cfg_marker_config_section_begin)
    {
        auto msg = std::string("Expected '") + cfg_marker_config_section_begin + "'";
        abort_parsing(*begin, msg);
    }

    // Given iterator seems legit, continue reading
    ++line;
    for (; line != end; ++line)
    {
        // Valid marker are:
        // 1) Field order
        // 2) End of config
        marker = get_line_marker(*line);

        // 1) Read field order
        if (marker == cfg_marker_config_field_order)
        {
            auto view = get_line_value(*line, cfg_marker_config_field_order);
            section.field_order = std::move(std::string(view.begin(), view.end()));
        }

        // 2) Read section end. Assign read section and return.
        else if(marker == cfg_marker_config_section_end)
        {
            cfg.global = std::move(section);
            return;
        }
        // Anything else is an error
        else
        {
            abort_parsing(*line, "Unexpected config entry");
        }
    }

    // In case we end up here: This is an error, the section was not closed. Abort.
    abort_parsing(*begin, "Config section was never closed");
}

// Read entire configuration contents into config structure
void read_config(std::vector<Line> const& cfg_file_content, Config& cfg)
{
    // Read config file contents line by line
    auto begin = cfg_file_content.begin();
    auto end = cfg_file_content.cend();

    for (auto line = begin; line != end; ++line)
    {
        // Only valid marker here are either:
        // 1) Begin of Config
        // 2) Begin of Group
        auto marker = get_line_marker(*line);

        if (marker == cfg_marker_config_section_begin)
        {
            read_config_section(line, end, cfg);
        }

        else if (marker == cfg_marker_group_section_begin)
        {
            read_group_section(line, end, cfg);
        }
        else
        {
            abort_parsing(*line, "Unexpected config entry");
        }
    }
}

// Verify host section in config structure
void verify_host_section(ConfigHost const& host)
{
    // Check if FQHN is set. Manditory.
    if (host.fqhn.empty())
    {
        abort("A host is missing the manditory field: FQHN");
    }

    // Check if INTERVAL is set. Manditory.
    if (host.interval.empty())
    {
        abort("A host is missing the manditory field: INTERVAL");
    }

    // Verify that interval is a positive number.
    auto interval = string_to_int(host.interval);
    if (interval)
    {
        auto val = interval.value();
        if (val <= 0)
        {
            abort("A hosts inverval is less or equal zero");
        }
    }
    else
    {
        abort("A hosts interval is not a number");
    }

    // Check if PROTOCOL is set. Manditory.
    if (host.protocol.empty())
    {
        abort("A host is missing the manditory field: PROTOCOL");
    }

    // Verify given Protocol
    auto proto = string_to_protocol(host.protocol);
    if (proto)
    {
        // In case of TCP, a valid port must be set
        if (proto.value() == Endpoint::Protocol::TCP)
        {
            // Check if PORT is set. Manditory if PROTOCOL is TCP.
            if (!host.port)
            {
                abort("A host is missing the manditory field: PORT. Only for TCP hosts");
            }

            // Verfiy that given port is a valid port.
            auto port = string_to_int(host.port.value());
            if (port)
            {
                if ((port <= 0) || (0xFFFF < port))
                {
                    abort("A hosts port is not in range: [1: 65535]");
                }
            }
            else
            {
                abort("A hosts port is not a number");
            }
        }
    }
    else
    {
        abort("A hosts protocol is neither ICMPV4, ICMPV6 nor TCP.");
    }
}

// Verify group section in config structure
void verify_group_section(ConfigGroup const& grp)
{
    // Check hosts. At least one host must be specified per group
    if (grp.hosts.empty())
    {
        abort("At least on group has no associated hosts");
    }

    for (auto const& host : grp.hosts)
    {
        verify_host_section(host);
    }
}

// Verify config section in config structure
void verify_config_section(ConfigGlobal const& cfg)
{
    // Check field order (manditory)
    if (cfg.field_order.empty())
    {
        auto msg = std::string("Config verification failed. Manditory field '");
        msg += cfg_marker_config_field_order;
        msg += "' is missing.";
        abort(msg);
    }

    // Lamba to test if a tken is valid.
    auto test_token = [] (std::string_view const& token)
    {
        if (string_to_field(token) == Field::Undef)
        {
            auto msg = std::string("Unknown field '");
            msg += token;
            msg += "' encountered in ";
            msg += cfg_marker_config_field_order;
            abort(msg);
        }
    };

    // There was a specified field order. Perform sanity checks.
    // Check that only known fields are in the field order.
    auto view = std::string_view(cfg.field_order);
    auto pos = std::size_t(0);

    while ((pos = view.find(cfg_delimiter)) != view.npos)
    {
        // Get Token from field order
        auto token = view.substr(0, pos);

        test_token(token);

        // Remove tested token and trim result.
        view.remove_prefix(token.size());
        view = trim_view(view);
    }

    // There the should be a token left in view.
    if (view.empty() == false)
    {
        test_token(view);
    }
}

// Verify entire config structure
void verify_config(Config const& cfg)
{
    verify_config_section(cfg.global);

    // At least one group has to be added.
    if (cfg.groups.empty())
    {
        abort("Configuration containes no group.");
    }

    // Verify each group
    for (auto const& grp : cfg.groups)
    {
        verify_group_section(grp);
    }
}

// Determine the field types, the ui consists of, by interpreting the field order.
void get_field_types_in_use( std::string const&                   field_order
                           , std::vector<ConfigGlobal::FieldFmt>& fmt
                           )
{
    // Add an entry to fmt for each field in field_order
    auto view = std::string_view(field_order);
    auto pos = std::size_t(0);

    auto add_token = [&fmt] (std::string_view const& token)
    {
        fmt.push_back(ConfigGlobal::FieldFmt(string_to_field(token), 0));
    };

    // Add all fields in view to vector
    while ((pos = view.find(cfg_delimiter)) != view.npos)
    {
        // Get Token from field order
        auto token = view.substr(0, pos);

        add_token(token);

        // Remove tested token and trim result.
        view.remove_prefix(token.size());
        view = trim_view(view);
    }

    // There the should be a token left in view.
    if (view.empty() == false)
    {
        add_token(view);
    }
}

// Get length of the static ui fields
unsigned get_field_length_ui_fields(Field field)
{
    switch(field)
    {
        case Field::Fqhn:     return std::strlen(ui_header_fqhn);
        case Field::Alias:    return std::strlen(ui_header_alias);
        case Field::Role:     return std::strlen(ui_header_role);
        case Field::Device:   return std::strlen(ui_header_device);
        case Field::Interval: return std::strlen(ui_header_interval);
        case Field::Protocol: return std::strlen(ui_header_protocol);
        default:              return 0;
    }
}

// Get length of the loaded ui fields
unsigned get_field_length_host(ConfigHost const& host, Field field)
{
    auto result = 0u;

    switch (field)
    {
        case Field::Undef:
            break;
        case Field::Fqhn:
            result = static_cast<unsigned>(host.fqhn.size());
            break;
        case Field::Alias:
            result = static_cast<unsigned>(host.alias.value_or("").size());
            break;
        case Field::Role:
            result = static_cast<unsigned>(host.role.value_or("").size());
            break;
        case Field::Device:
            result = static_cast<unsigned>(host.device.value_or("").size());
            break;
        case Field::Interval:
            result = static_cast<unsigned>(make_interval_string(host.interval).size());
            break;
        case Field::Protocol:
            result = static_cast<unsigned>(make_proto_port_string(host.protocol, host.port).size());
            break;
    }

    return result;
}

// Get minimum length of a given Field to display it properly.
void get_field_length( std::vector<ConfigGroup> const& groups
                     , Field const&                    field
                     , ConfigGlobal::FieldLen&         field_len
                     )
{
    // Get length of ui fields
    field_len = get_field_length_ui_fields(field);

    // Iterate over all hosts. Search for largest string of of field
    for (auto const& grp : groups)
    {
        for (auto const& host: grp.hosts)
        {
            field_len = std::max(field_len, get_field_length_host(host, field));
        }
    }
}

// Calculate visible fields and thier length from read config.
void generate_field_format(Config& cfg)
{
    auto fmt = std::vector<ConfigGlobal::FieldFmt>();
    get_field_types_in_use(cfg.global.field_order, fmt);

    // Get the max length from field values.
    for (auto& [type, length] : fmt)
    {
        get_field_length(cfg.groups, type, length);
    }

    cfg.global.field_format = std::move(fmt);
}
} // anon namespace

// Read and verify the configuration file.
Config read_config_file(std::string const& cfg_file_path)
{
    auto cfg_file_content = std::vector<Line>();
    auto cfg = Config();

    load_config_file(cfg_file_path, cfg_file_content);

    // Stage 1: Read config file contents into config data structure
    read_config(cfg_file_content, cfg);

    // Stage 2: Verify config data structure
    verify_config(cfg);

    // Stage 3: Setup field format from config data structure
    generate_field_format(cfg);

    return cfg;
}

// Straight forward implementations: Nothing special here ;)
std::string field_to_string(Field field)
{
    switch(field)
    {
        case Field::Fqhn:     return "FQHN";
        case Field::Alias:    return "ALIAS";
        case Field::Role:     return "ROLE";
        case Field::Device:   return "DEVICE";
        case Field::Protocol: return "PROTOCOL";
        case Field::Interval: return "INTERVAL";
        default:              return "UNDEF";
    }
}

Field string_to_field(std::string_view const& str)
{
    if (str == "FQHN")
    {
        return Field::Fqhn;
    }
    if (str == "ALIAS")
    {
        return Field::Alias;
    }
    if (str == "ROLE")
    {
        return Field::Role;
    }
    if (str == "DEVICE")
    {
        return Field::Device;
    }
    if (str == "PROTOCOL")
    {
        return Field::Protocol;
    }
    if (str == "INTERVAL")
    {
        return Field::Interval;
    }
    return Field::Undef;
}


// Config Streaming operators
std::ostream& operator << (std::ostream& ost, Config const& cfg)
{
    ost << "Config[global='" << cfg.global;
    ost << "', groups=[";

    for (auto it = cfg.groups.begin(); it != cfg.groups.end(); ++it)
    {
        ost << *it << ", ";
    }

    ost << "]]";
    return ost;
}

std::ostream& operator << (std::ostream& ost, ConfigGlobal const& cfg)
{
    ost << "ConfigGlobal[field_order='" << cfg.field_order;
    ost << "', field_format=[";
    for (auto const& [field, len] : cfg.field_format)
    {
        ost << "[" << field_to_string(field) << ", " << len << "], ";
    }
    ost << "]]";
    return ost;
}

std::ostream& operator << (std::ostream& ost, ConfigGroup const& cfg)
{
    ost << "ConfigGroup[name='" << cfg.name.value_or("");
    ost << "', hosts=[";

    for (auto it = cfg.hosts.begin(); it != cfg.hosts.end(); ++it)
    {
        ost << *it << ", ";
    }
    ost << "]]";

    return ost;
}

std::ostream& operator << (std::ostream& ost, ConfigHost const& cfg)
{
    ost << "ConfigHost[fqhn='" << cfg.fqhn;
    ost << "', alias='" << cfg.alias.value_or("");
    ost << "', role='" << cfg.role.value_or("");
    ost << "', device='" << cfg.device.value_or("");
    ost << "', protocol='" << cfg.protocol;
    ost << "', port='" << cfg.port.value_or("");
    ost << "', interval='" << cfg.interval;
    ost << "']";
    return ost;
}
