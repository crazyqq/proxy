/*
 * httpconnection.cpp
 *
 *  Created on: Aug 27, 2014
 *      Author: feasty
 */
#include <string>
#include <unistd.h>
#include <iostream>

#include "httpconnection.hpp"

using namespace std;

#define CLIENTBUFFERSIZE 1024

namespace connection
{

HttpConnection::HttpConnection()
{

}

HttpConnection::~HttpConnection()
{

}

//
// Process the http request
//
bool HttpConnection::process_request(string message, int &client_socket)
{
    bool result = true;

    result = m_request_parser.parse(message);

    if(result)
    {
        HttpRequest http_req = m_request_parser.get_bttp_request();
        string response;
        connect_to_web_server(http_req, response);

        if(!response.empty())
        {
            cout << "Writing response to client: \n"<< response << endl;
            write(client_socket, response.c_str(), sizeof(response.c_str()));
        }
    }

    return result;
}

//
// Creates the connection to the web server.
//
void HttpConnection::connect_to_web_server(HttpRequest &http_req, string &response)
{
    bool success = true;

    response.clear();

    struct sockaddr_in web_sock_info;
    struct hostent *web_server_addr;

    bzero(&web_sock_info, sizeof(web_sock_info));

    web_server_addr = gethostbyname(http_req.get_url().c_str());

    if(web_server_addr == NULL)
    {
        cout << "Error resolving address " << http_req.get_url() << endl;
        success = false;
    }

    web_sock_info.sin_family = AF_INET;
    web_sock_info.sin_port = htons(http_req.get_web_server_port());
    bcopy((char *)web_server_addr->h_addr, (char *)&web_sock_info.sin_addr.s_addr, web_server_addr->h_length);

    int web_socket = socket(AF_INET, SOCK_STREAM, 0);

    cout << "Url: " << http_req.get_url() << "\nPort: " << http_req.get_web_server_port() << endl;

    if(web_socket == -1)
    {
        cout << "Could not create socket!" << endl;
    }

    success = connect(web_socket, (struct sockaddr*) &web_sock_info, sizeof(web_sock_info));

    if(success == 0)
    {
        cout << "Connected to web server successfully." << endl;
    }
    else
    {
        cout << "There was a problem connecting to the web server" << endl;
    }

    char rcv_buf[CLIENTBUFFERSIZE];

    const string request = http_req.get_request();

    success = write(web_socket, request.c_str(), sizeof(request.c_str()));

    if(success)
    {
        cout << "Sent request to server." << endl;
    }

    int num_read = recv(web_socket, rcv_buf, CLIENTBUFFERSIZE, 0);

    cout << num_read << endl;
    if(num_read > 0)
    {
        rcv_buf[num_read] = '\0';
        cout << rcv_buf << endl;
    }

    cout << "Received: \n" << rcv_buf << endl;

    cout << response << endl;
}

}
