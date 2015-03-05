/*
 * utils.hpp
 *
 *  Created on: Aug 29, 2014
 *      Author: feasty
 */
#include <string>

#ifndef UTILS_HPP_
#define UTILS_HPP_

namespace textutils
{

//
// @brief   Takes a string and converts all characters to lower case.
//
// @param[in]   &in  The string containing upper/mixed characters.
// @param[out]  &out The string converted to lower case.
//
void to_lower(const std::string &in, std::string &out);

}

#endif /* UTILS_HPP_ */
