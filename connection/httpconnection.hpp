/*
 * httpconnection.hpp
 *
 *  Created on: Aug 27, 2014
 *      Author: feasty
 */

#include "connectionbase.hpp"

#ifndef HTTPCONNECTION_HPP_
#define HTTPCONNECTION_HPP_

namespace connection
{

class HttpConnection : public ConnectionBase
{
public:
    //
    //  @brief  The constructor.
    //
    HttpConnection();

    //
    //  @brief  The destructor.
    //
    ~HttpConnection();

private:

    //
    // @brief   Process the http request.
    //
    // @param   &message    The http request to process.
    //
    //  @return     Returns true if the operation was successful.
    //
    bool process_request(std::string message, int &client_socket);

    //
    // @brief   Creates the connection to the web server.
    //
    void connect_to_web_server(HttpRequest &http_req, std::string &response);
};

}

#endif /* HTTPCONNECTION_HPP_ */
