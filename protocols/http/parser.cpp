/*
 * parser.cpp
 *
 *  Created on: Aug 25, 2014
 *      Author: feasty
 */
#include <iostream>

#include "parser.hpp"

using namespace std;

Parser::Parser():
        m_current_state(BEGIN_REQUEST_RESPONSE_STATE)
{

}

Parser::~Parser()
{

}

void Parser::reset()
{

}

bool Parser::is_valid_http_character(const char &c)
{
    bool is_valid = false;

    // Validates the value range of the byte as a valid request
    // character, excluding space and crlf.
    if((c >= 0x2D && c <= 0x3B) ||
        (c >= 0x41 && c <= 0x5B) ||
        (c >= 0x61 && c <= 0x7A) ||
        c == 0x22)
    {
        is_valid = true;
    }

    return is_valid;
}

bool Parser::parse(const string &buf)
{
    bool is_ok = true;
    m_current_state = BEGIN_REQUEST_RESPONSE_STATE;

    for(const char& current_char : buf)
    {
        switch(m_current_state)
        {
            case BEGIN_REQUEST_RESPONSE_STATE :
                begin_request_reponse_state(current_char, is_ok);
                break;
            case REQUEST_OR_RESPONSE_STATE :
                request_or_response_state(current_char, is_ok);
                break;
            case REQUEST_OR_RESPONSE_SPACE_STATE :
                request_or_response_space_state(current_char, is_ok);
                break;
            case HEADER_NAME_STATE :
                header_name_state(current_char, is_ok);
                break;
            case COLON_DELIMETER_STATE :
                colon_delimeter_state(current_char, is_ok);
                break;
            case HEADER_VALUE_STATE :
                header_value_state(current_char, is_ok);
                break;
            case NEWLINE_STATE :
                newline_state(current_char, is_ok);
                break;
            case END_REQUEST_STATE :
                end_request_state(current_char, is_ok);
                break;
        }
        if (!is_ok)
        {
            break;
        }
    }

    if (m_current_state == BEGIN_REQUEST_RESPONSE_STATE)
    {
        is_ok = false;
    }

    return is_ok;
}

void Parser::begin_request_reponse_state(const char &current_char, bool &is_ok)
{
    if(is_valid_http_character(current_char))
    {
        m_process_buf += current_char;
        m_current_state = REQUEST_OR_RESPONSE_STATE;
    }
}

void Parser::header_name_state(const char &current_char, bool &is_ok)
{
    if(current_char == ':')
    {
        process_header(m_process_buf, is_ok);

        m_process_buf.clear();
        m_current_state = COLON_DELIMETER_STATE;
    }
    else if(is_valid_http_character(current_char))
    {
        m_process_buf += current_char;
    }
}

void Parser::colon_delimeter_state(const char &current_char, bool &is_ok)
{
    if(is_valid_http_character(current_char))
    {
        m_process_buf += current_char;
        m_current_state = HEADER_VALUE_STATE;
    }
}

void Parser::header_value_state(const char &current_char, bool &is_ok)
{
    if((current_char == '\r') || (current_char == '\n'))
    {
        process_header(m_process_buf, is_ok);

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
        m_process_buf += current_char;
    }
}

void Parser::newline_state(const char &current_char, bool &is_ok)
{
    if(current_char == '\n')
    {
        m_current_state = HEADER_NAME_STATE;
    }
    else
    {
        is_ok = false;
    }
}

void Parser::end_request_state(const char &current_char, bool &is_ok)
{

}
