/**
 * @file      Args.hpp
 * @author    Simon Brummer (<simon.brummer@posteo.de>)
 * @brief     Command line argument parsing.
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef ARGS_HPP_201804081223
#define ARGS_HPP_201804081223

#include <map>
#include <string>

// Read command line arguments
std::map<std::string, std::string> read_args(int cargc, char **cargv);

#endif // ARGS_HPP_201804081223
