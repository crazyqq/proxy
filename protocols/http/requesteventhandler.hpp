/*
 * requesteventhandler.hpp
 *
 *  Created on: Aug 25, 2014
 *      Author: feasty
 */
#include <string>

#include "requestparser.hpp"
#include "httprequest.hpp"

#ifndef REQUEST_EVENT_HANDLER_HPP
#define REQUEST_EVENT_HANDLER_HPP

class RequestEventHandler : public RequestParser
{
public:
    //
    // @brief   The constructor
    //
    RequestEventHandler();

    //
    // @brief   The destructor
    //
    virtual ~RequestEventHandler();

    //
    // @brief   Returns the http request object.
    //
    HttpRequest& get_bttp_request();

protected:
    void process_request(const std::string &request_buf, bool &is_ok);

    void process_header(const std::string &header_buf, bool &is_ok);

private:
    std::string m_request_method;
    std::string m_request_resource;
    std::string m_request_version;

    std::string m_header_name;
    std::string m_header_value;

    HttpRequest m_http_request;

    //
    // @brief   Returns the url and port the request will use to connect to the web server.
    //
    // @param[in]   &buf    The string containing the hostname and port to parse.
    // @param[out]  &url    The url extracted from the input.
    // @param[out]  &port   The port extracted from the input.
    //
    // @return  Returns the host and port of the web server
    //
    void extract_url_and_port(const std::string &buf, std::string &url, int &port);

    //
    // @brief   Sets the URL and port member values extracted from buf.
    //
    // @param[in]   &buf    The string containing the hostname and port to parse.
    //
    void set_url_and_port(const std::string &buf);
};

#endif // REQUEST_EVENT_HANDLER_HPP
