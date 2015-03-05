/*
 * utils.cpp
 *
 *  Created on: Aug 29, 2014
 *      Author: feasty
 */
#include "utils.hpp"

using namespace std;

#define LOWEROFFSET 32

void textutils::to_lower(const string &in, string &out)
{
    string::const_iterator it;

    for(it = in.begin(); it != in.end(); ++it)
    {
        if((*it >= 65) && (*it <= 90))
        {
            out += (*it + LOWEROFFSET);
        }
        else
        {
            out += *it;
        }
    }
}
