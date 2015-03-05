/*
 * requesteventhandler.cpp
 *
 *  Created on: Aug 25, 2014
 *      Author: feasty
 */
#include <iostream>

#include "requesteventhandler.hpp"
#include "utility/characterencoding/utils.hpp"

#define DEFAULTPORT 80

using namespace std;
using namespace textutils;

RequestEventHandler::RequestEventHandler():
                        m_request_method(""),
                        m_request_resource(""),
                        m_request_version(""),
                        m_header_name(""),
                        m_header_value("")
{

}

RequestEventHandler::~RequestEventHandler()
{

}

HttpRequest& RequestEventHandler::get_bttp_request()
{
    return m_http_request;
}

void RequestEventHandler::process_request(const std::string &request_buf, bool &is_ok)
{
    cout << "Request: " << request_buf << endl;

    if(m_request_method.empty())
    {
        m_request_method = request_buf;
    }
    else if(m_request_resource.empty())
    {
        m_request_resource = request_buf;

        set_url_and_port(request_buf);
    }
    else if(m_request_version.empty())
    {
        m_request_version = request_buf;
        m_http_request.add_request(m_request_method, m_request_resource, m_request_version);
    }
    // Shouldn't be here
    else
    {
        is_ok = false;
    }
}

void RequestEventHandler::process_header(const std::string &header_buf, bool &is_ok)
{
    cout << "Header: " << header_buf << endl;

    if(m_header_name.empty())
    {
        m_header_name = header_buf;
    }
    else if(m_header_value.empty())
    {
        m_header_value = header_buf;
        m_http_request.add_header(m_header_name, m_header_value);

        // If we found the host
        string hdr_lower;
        to_lower(m_header_name, hdr_lower);

        if(hdr_lower == "host")
        {
            set_url_and_port(header_buf);
        }

        m_header_name.clear();
        m_header_value.clear();
    }
}

//
// Returns the port the request will connect to on the web server.
//
void RequestEventHandler::extract_url_and_port(const std::string &buf, std::string &url, int &port)
{
    // TODO - url is http://news.bbc.co.uk:443/

    port = DEFAULTPORT;
    url.clear();
    string port_ascii("");
    bool found_delim = false;

    for(const char& current_char : buf)
    {
        // If we find the delimeter between the hostname and port
        // then we switch contexts.
        if(current_char == ':')
        {
            found_delim = true;
            continue;
        }

        if(found_delim)
        {
            port_ascii += current_char;
        }
        // If we find a / we break out of the loop as we have hit
        // www.example.org:443/foo/bar
        else if(current_char == '/')
        {
            break;
        }
        else
        {
            url += current_char;
        }
    }

    try
    {
        port = stoi(port_ascii);
    }
    catch(invalid_argument)
    {
        cout << "Cannot convert port number " << port_ascii << endl;
    }
}

void RequestEventHandler::set_url_and_port(const std::string &buf)
{
    string url("");
    int port = 0;

    extract_url_and_port(buf, url, port);

    cout << "Extracted URL is " << url << endl;
    cout << "Extracted port is " << endl;

    // Store the web server URL and port in the request.
    m_http_request.set_url(url);
    m_http_request.set_web_server_port(port);
}
