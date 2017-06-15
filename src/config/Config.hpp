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
 * @date   15.6.2017
 */

#ifndef CONFIG_HPP_201706151113
#define CONFIG_HPP_201706151113

#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include "HostMonitor.hpp"

// Constants
namespace
{
    std::string const defaultConfigFile = ".host_monitor";
    char const        commentMark = '#';
}

// Local functions.
namespace
{
    /**
     * @brief Trim whitespace character from both ends of a given string.
     * @param[in] s   string that should be trimmed
     * @returns   trimmed string
     */
    inline auto trim(std::string const& s) -> std::string
    {
        auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c){return std::isspace(c);});
        auto wsback  = std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
        return (wsback <= wsfront) ? std::string() : std::string(wsfront,wsback);
    }

    /**
     * @brief Print Usage to commandline.
     */
    inline auto printUsage() -> void
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
}

/**
 * @brief Read command line arguments
 * @param[in] argc   Number of supplied arguments
 * @param[in] argv   Supplied arguments
 * @returns   Map holding set options and values.
 */
inline auto readArgs(int cargc, char **cargv) -> std::map<std::string, std::string>
{
    std::map<std::string, std::string> args;

    // Copy arguments into c++ structures, remove executable name.
    std::vector<std::string> argv;
    for (int i = 0; i < cargc; ++i)
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
            printUsage();
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
                std::cerr << "Option -f is missing a path. Abort" << std::endl;
                printUsage();
                exit(-1);
            }
        }

        // Unknown option abort
        else
        {
            std::cerr << "Unknown parameter '" << *it << "' abort" << std::endl;
            printUsage();
            exit(-1);
        }
    }

    // Set default parameter if they were not specified.
    // Try to load default configuration file.
    auto pos = args.find("-f");
    if (pos == args.end())
    {
        char *env_home = getenv("HOME");
        if (env_home == nullptr)
        {
            throw std::runtime_error("$HOME is not set. Abort operation.");
        }
        args["-f"] = std::string(env_home) + "/" + defaultConfigFile;
    }
    return args;
}

/**
 * @brief Read configuration file
 * @param[in] path   absolute path to configuration file to load.
 * @returns   Read host monitor objects
 */
inline auto readConfigFile(std::string const& path) -> std::vector<std::shared_ptr<HostMonitor>>
{
    // Try to open given configuration file.
    std::ifstream ifs(path.c_str(), std::ifstream::in);
    if (!ifs.is_open())
    {
       std::cerr << "Can't open config file '" << path << "'" << std::endl;
       exit(-1);
    }

    // Read configuration file line by line.
    std::vector<std::string> lines;
    while(ifs)
    {
        std::string line;
        std::getline(ifs, line);

        //Strip comments from line
        auto pos = line.find(commentMark);
        if (pos != line.npos)
        {
            line.erase(line.begin() + pos, line.end());
        }

        // Trim Line, discard if it is empty
        line = trim(line);
        if (line.empty())
        {
            continue;
        }

        lines.push_back(line);
    }
    ifs.close();

    // Iterate over lines, extracting configured parameters.
    bool host     = false;
    bool hostname = false;
    bool hostport = false;
    bool protocol = false;
    bool interval = false;

    std::string          strHost;
    std::string          strHostname;
    uint16_t             valHostport = 0;
    Protocol             valProtocol = Protocol::ICMP;
    std::chrono::seconds valInterval;

    std::vector<std::shared_ptr<HostMonitor>> mon;
    for (auto it = lines.begin(); it != lines.end(); ++it)
    {
        // Check for tag: 'Host:'
        auto pos = it->find("Host:");
        if (pos != it->npos)
        {
            // Extract value, set flag
            it->erase(it->begin(), it->begin() + sizeof("Host:"));
            strHost = trim(*it);
            host = true;
        }

        // Check for tag: 'HostName:'
        pos = it->find("HostName:");
        if (pos != it->npos)
        {
            // Extract value, set flag
            it->erase(it->begin(), it->begin() + sizeof("HostName:"));
            strHostname = trim(*it);
            hostname = true;
        }

        // Check for tag: 'HostPort:'
        pos = it->find("HostPort:");
        if (pos != it->npos)
        {
            // Extract value, set flag
            it->erase(it->begin(), it->begin() + sizeof("HostPort:"));
            std::string strHostport = trim(*it);

            try
            {
                // Convert string to int and check if int is sane boundries
                int val = std::stoi(strHostport);
                int min = std::numeric_limits<uint16_t>::min();
                int max = std::numeric_limits<uint16_t>::max();

                if (min <= val && val <= max)
                {
                    valHostport = val;
                    hostport = true;
                }
                else
                {
                    // Will be catched below
                    throw std::runtime_error("out-of-bounds");
                }
            }
            catch (std::invalid_argument& e)
            {
                std::cerr << "Invalid Port: '" << strHostport << "'" << std::endl;
                exit(-1);
            }

        }

        // Check for tag: 'Protocol:'
        pos = it->find("Protocol:");
        if (pos != it->npos)
        {
            // Extract value, set flag
            it->erase(it->begin(), it->begin() + sizeof("Protocol:"));
            std::string strProtocol = trim(*it);
            std::cout << strProtocol << std::endl;

            // Check Protocol
            if (strProtocol == "icmp")
            {
                // ICMP Only: mark port for as true. Its not evaluated anyway.
                valProtocol = Protocol::ICMP;
                valHostport = 0;
                hostport = true;
            }
            else if (strProtocol == "tcp")
            {
                valProtocol = Protocol::TCP;
            }
            // Unknown Protocol. Abort
            else
            {
                std::cerr << "Unknown Protocol: '" << strProtocol << "'" << std::endl;
                exit(-1);
            }
            protocol = true;
        }

        // Check for tag: 'Interval:'
        pos = it->find("Interval:");
        if (pos != it->npos)
        {
            // Extract value, set flag
            it->erase(it->begin(), it->begin() + sizeof("Interval:"));
            std::string strInterval = trim(*it);

            try
            {
                auto val = std::stoi(strInterval);
                valInterval = std::chrono::seconds(val);
                interval = true;
            }
            catch (std::invalid_argument& e)
            {
                std::cerr << "Invalid Interval: '" << strInterval << "'" << std::endl;
                exit(-1);
            }
        }

        // Assemble host monitor if complete
        if (host && hostname && hostport && protocol && interval)
        {
            // Clear flags
            host = hostname = hostport = protocol = interval = false;

            std::shared_ptr<HostMonitor> m;
            switch(valProtocol)
            {
            case Protocol::ICMP:
                m = makeHostMonitor(makeIcmpEndpoint(strHostname), strHost, valInterval);
                break;

            case Protocol::TCP:
                m = makeHostMonitor(makeTcpEndpoint(strHostname, valHostport), strHost, valInterval);
                break;
            }
            mon.push_back(m);
        }
    }
    return mon;
}

#endif // CONFIG_HPP_201706151113
