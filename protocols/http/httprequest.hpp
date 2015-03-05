/*
 * httprequest.hpp
 *
 *  Created on: Aug 25, 2014
 *      Author: feasty
 */
#include <map>
#include <string>

#ifndef HTTPREQUEST_HPP_
#define HTTPREQUEST_HPP_

class HttpRequest
{
public:
    HttpRequest();

    ~HttpRequest();

    void add_request(const std::string &method, const std::string &resource, const std::string &version);

    void add_request_method(const std::string method);

    void add_request_resource(const std::string resource);

    void add_resource_version(const std::string version);

    std::string get_request_method();

    std::string get_request_resource();

    std::string get_request_version();

    void add_header(const std::string &header_name, const std::string &header_value);

    const std::map<std::string, std::string>& get_headers();

    const std::string get_request();

    const std::string& get_url();

    const int get_web_server_port();

    void set_url(std::string url);

    void set_web_server_port(int port);

private:
    //
    // @struct  m_request
    //
    // @brief   Holds the request information.
    //
    struct
    {
        std::string method;
        std::string resource;
        std::string version;
    } m_request;

    //
    // @var     m_headers
    //
    // @brief   Map containing the request headers
    //
    std::map<std::string, std::string> m_headers;

    std::string m_web_url;

    int m_web_server_port;

};

#endif // HTTPREQUEST_HPP
