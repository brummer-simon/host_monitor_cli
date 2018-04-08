/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   Args.cpp
 * @author Simon Brummer
 * @date   08.04.2018
 */

#include <vector>
#include <iostream>
#include "Constants.hpp"
#include "Util.hpp"
#include "Args.hpp"

namespace
{
auto print_usage() -> void
{
    std::cout << "\n";
    std::cout << "Usage:\n";
    std::cout << "    host_monitor_cli [-h] [-f <path>]\n";
    std::cout << "\n";
    std::cout << "Options:\n";
    std::cout << "    -f <path>   User specified configuration file\n";
    std::cout << "    -h          Print this help\n";
    std::cout << std::endl;
}
} // namespace anon

auto read_args(int cargc, char **cargv) -> std::map<std::string, std::string>
{
    auto args = std::map<std::string, std::string>();
    auto argv = std::vector<std::string>();

    // Copy arguments into c++ structures, remove executable name.
    for (auto i = int(0); i < cargc; ++i)
    {
      argv.push_back(cargv[i]);
    }
    argv.erase(argv.begin());

    // Parse arguments
    for (auto it = argv.cbegin(); it != argv.cend(); ++it)
    {
        // Examine -h option
        if (*it == "-h")
        {
            print_usage();
            exit(0);
        }
        // Examine -f Option
        else if (*it == "-f")
        {
            // Add the following string as argument, if there is one
            if (++it != argv.cend())
            {
                args["-f"] = *it;
            }

            // Missing operand abort.
            else
            {
                abort("Option -f is missing a path. Abort");
            }
        }

        // Unknown option abort
        else
        {
            abort("Unknown parameter '" + *it + "'. Abort");
        }
    }

    // Set default parameter if they were not specified.
    // Try to load default configuration file.
    auto pos = args.find("-f");
    if (pos == args.end())
    {
        auto env_home = getenv("HOME");
        if (env_home == nullptr)
        {
            abort("$HOME is not set. Abort operation.");
        }
        args["-f"] = std::string(env_home) + "/" + cfg_default_config_file;
    }
    return args;
}