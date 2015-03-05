/*
 * parser.hpp
 *
 *  Created on: Aug 24, 2014
 *      Author: feasty
 */

#include "parser.hpp"

#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

class RequestParser : public Parser
{
public:
	//
	// @brief	Default constructor
	//
	RequestParser();

	//
	// @brief	Destructor
	//
	virtual ~RequestParser();

protected:
	virtual void process_request(const std::string &request_buf, bool &is_ok) = 0;

	virtual void process_header(const std::string &header_buf, bool &is_ok) = 0;

	void request_or_response_state(const char &current_char, bool &is_ok);

	void request_or_response_space_state(const char &current_char, bool &is_ok);
};

#endif // REQUEST_PARSER_HPP
