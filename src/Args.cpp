/**
 * @file      Args.cpp
 * @author    Simon Brummer (<simon.brummer@posteo.de>)
 * @brief     Command line argument parsing.
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <vector>
#include <iostream>
#include "Constants.hpp"
#include "Util.hpp"
#include "Args.hpp"
#include "Version.hpp"

namespace
{
void print_usage()
{
    std::cout << "\n";
    std::cout << "Usage:\n";
    std::cout << "    host_monitor_cli [-h] [-f <path>]\n";
    std::cout << "\n";
    std::cout << "Options:\n";
    std::cout << "    -f <path>   User specified configuration file\n";
    std::cout << "    -h          Print this help\n";
    std::cout << "    -v          Print Version Information\n";
    std::cout << std::endl;
}

void print_version()
{
    std::cout << "host_monitor_cli " << Version::full << "\n";
    std::cout << "Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>\n";
    std::cout << "\n";
    std::cout << "This file is subject to the terms and conditions of the GNU Lesser General Public License v2.1.\n";
    std::cout << "This is free software: you are free to change and redistribute it.\n";
    std::cout << "There is NO WARRANTY, to the extent permitted by law.\n";
    std::cout << "\n";
    std::cout << "Written by Simon Brummer.";
    std::cout << std::endl;
}

} // namespace anon

std::map<std::string, std::string> read_args(int cargc, char **cargv)
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
        // Examine -v option
        else if (*it == "-v")
        {
            print_version();
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
