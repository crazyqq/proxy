/*
 * parser.cpp
 *
 *  Created on: Aug 24, 2014
 *      Author: feasty
 */
#include <iostream>

#include "requestparser.hpp"

RequestParser::RequestParser()
{

}

RequestParser::~RequestParser()
{

}

void RequestParser::request_or_response_state(const char &current_char, bool &is_ok)
{
    if(is_valid_http_character(current_char))
    {
        m_process_buf += current_char;
    }
    else if(current_char == ' ')
    {
        process_request(m_process_buf, is_ok);

        m_process_buf.clear();
        m_current_state = REQUEST_OR_RESPONSE_SPACE_STATE;
    }
    else if((current_char == '\r') || (current_char == '\n'))
    {
        process_request(m_process_buf, is_ok);

        m_process_buf.clear();

        if(current_char == '\r')
        {
            m_current_state = NEWLINE_STATE;
        }
        else
        {
            m_current_state = HEADER_NAME_STATE;
        }
    }
    else
    {
        is_ok = false;
    }
}

void RequestParser::request_or_response_space_state(const char &current_char, bool &is_ok)
{
    if(is_valid_http_character(current_char))
    {
        m_process_buf += current_char;
        m_current_state = REQUEST_OR_RESPONSE_STATE;
    }
}
