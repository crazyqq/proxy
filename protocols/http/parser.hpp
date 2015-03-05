/*
 * parser.hpp
 *
 *  Created on: Aug 25, 2014
 *      Author: feasty
 */

#import <string>

#ifndef PARSER_HPP
#define PARSER_HPP

class Parser
{
public:
    //
    // @brief   Default constructor
    //
    Parser();

    //
    // @brief   Destructor
    //
    virtual ~Parser();

    //
    // @brief   Resets the parser.
    //
    void reset();

    virtual bool parse(const std::string &buf);

protected:
    //
    // @var     process_buf
    //
    // @brief   Buffer used to store data for processing by the event
    //          handler.
    //
    std::string m_process_buf;

    bool is_valid_http_character(const char &c);

    //
    // @enum    state
    //
    // @brief   Parser states
    //
    enum state
    {
        BEGIN_REQUEST_RESPONSE_STATE,
        REQUEST_OR_RESPONSE_STATE,
        REQUEST_OR_RESPONSE_SPACE_STATE,
        HEADER_NAME_STATE,
        COLON_DELIMETER_STATE,
        HEADER_VALUE_STATE,
        NEWLINE_STATE,
        END_REQUEST_STATE
    };

    //
    // @var    m_current_state
    //
    // @brief   The current state of parsing the request.
    //
    state m_current_state;

    void begin_request_reponse_state(const char &current_char, bool &is_ok);

    virtual void request_or_response_state(const char &current_char, bool &is_ok) = 0;

    virtual void request_or_response_space_state(const char &current_char, bool &is_ok) = 0;

    void header_name_state(const char &current_char, bool &is_ok);

    void colon_delimeter_state(const char &current_char, bool &is_ok);

    void header_value_state(const char &current_char, bool &is_ok);

    void newline_state(const char &current_char, bool &is_ok);

    void end_request_state(const char &current_char, bool &is_ok);

    virtual void process_header(const std::string &header_buf, bool &is_ok) = 0;
};

#endif // PARSER_HPP
