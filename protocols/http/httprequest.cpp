/*
 * httprequest.cpp
 *
 *  Created on: Aug 25, 2014
 *      Author: feasty
 */

#include "httprequest.hpp"

#include <iostream>

using namespace std;

#define HDR_DELIMETER ": "
#define HDR_ENDLINE "\r\n"

HttpRequest::HttpRequest():
        m_web_url(""),
        m_web_server_port(0)
{
    m_request.method = "";
    m_request.resource = "";
    m_request.version = "";
}

HttpRequest::~HttpRequest()
{

}

void HttpRequest::add_request(const string &method, const string &resource, const string &version)
{
    m_request.method = method;
    m_request.resource = resource;
    m_request.version = version;
}

void HttpRequest::add_request_method(const string method)
{
    m_request.method = method;
}

void HttpRequest::add_request_resource(const string resource)
{
    m_request.resource = resource;
}

void HttpRequest::add_resource_version(const string version)
{
    m_request.version = version;
}

string HttpRequest::get_request_method()
{
    return m_request.method;
}

string HttpRequest::get_request_resource()
{
    return m_request.resource;
}

string HttpRequest::get_request_version()
{
    return m_request.version;
}

void HttpRequest::add_header(const string &header_name, const string &header_value)
{
    m_headers[header_name] = header_value;
}

const map<string, string>& HttpRequest::get_headers()
{
    return m_headers;
}

const string HttpRequest::get_request()
{
    string header = m_request.method + " "
            + m_request.resource + " "
            + m_request.version + HDR_ENDLINE;

    map<string, string>::iterator hdr_iter;

    for(hdr_iter = m_headers.begin(); hdr_iter != m_headers.end(); ++hdr_iter)
    {
        header.append(hdr_iter->first);
        header.append(HDR_DELIMETER);
        header.append(hdr_iter->second);
        header.append(HDR_ENDLINE);
    }

    cout << "Request: ********" << endl;
    cout << header << endl;
    cout << "End request: ***********" << endl;

    return header;
}

const std::string& HttpRequest::get_url()
{
    return m_web_url;
}

const int HttpRequest::get_web_server_port()
{
    return m_web_server_port;
}


void HttpRequest::set_url(std::string url)
{
    m_web_url = url;
}

void HttpRequest::set_web_server_port(int port)
{
    m_web_server_port = port;
}
