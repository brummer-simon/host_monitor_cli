/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   Args.hpp
 * @author Simon Brummer
 * @date   08.04.2018
 */

#ifndef ARGS_HPP_201804081223
#define ARGS_HPP_201804081223

#include <map>
#include <string>

// Read command line arguments
auto read_args(int cargc, char **cargv) -> std::map<std::string, std::string>;

#endif // ARGS_HPP_201804081223