#include <netdb.h>
#include <string>
#include <vector>
#include <map>

#include "base/types.hpp"
#include "protocols/http/requesteventhandler.hpp"

#ifndef CONNECTION_BASE_HPP
#define CONNECTION_BASE_HPP

namespace connection
{

class ConnectionBase
{
public:
    //
    // @brief   Default Constructor
    //
    ConnectionBase(void);

    //
    // @brief   Constructor
    //
    // @param   port    The port on which to listen.
    //
    ConnectionBase(Uint16 port);

    //
    // @brief   Constructor
    //
    // @param   port                The port on which to listen.
    // @param   number_of_threads   The number of threads to start.
    //
    ConnectionBase(Uint16 port, Uint16 number_of_threads);

    //
    // @brief   Destructor
    //
    virtual ~ConnectionBase(void);

    //
    // @brief   Creates a threadpool of listening threads
    //
    bool server_listen(void);

    //
    // @brief   Send a message to a specific client
    //
    // @param   &message    The message to send to the client.
    // @param   &socket     The socket ID on which to send the message to.
    //
    //  @return     Returns true if the operation was successful.
    //
    bool send_message_to_client(const std::string &message, const int &socket);

protected:

    //
    // @brief   Server thread which listens for incoming client connections.
    //
    void* server_thread(void);

    //
    // @brief   Helper function which simply calls server_thread() to encapsulate
    //          the threads.
    //
    // @param   *my_instance    Pointer to the ConnectionBase instance.
    //
    static void* server_thread_helper(void *my_instance);

    //
    // @brief   Process the http request.
    //
    // @param   &message    The http request to process.
    //
    //  @return     Returns true if the operation was successful.
    //
    virtual bool process_request(std::string message, int &client_socket) = 0;

    //
    // @brief   Closes the socket passed in and removes from the socket vector
    //
    // @param   &socket The socket on which to close the connection.
    //
    // @return  Returns true if successfully closed.
    //
    bool close_connection(int &socket);

    //
    // @brief   Closes all active connections to the server
    //
    bool close_all_connections();

    //
    // @var     m_request_parser
    //
    // @brief   Parser for the request.
    //
    RequestEventHandler m_request_parser;

private:
    //
    // @var     m_server_sock
    //
    // @type    int
    //
    // @brief   The main socket to listen for clients connections on.
    //
    int m_server_sock;

    //
    // @var	    m_client_sockets
    //
    // @type    vector<int>
    //
    // @brief    Vector of client sockets used in the threads.
    //
    std::vector<int> m_client_sockets;

    //
    // @var	    m_serverPort
    //
    // @type    int
    //
    // @brief	Port to listen on
    //
    int m_serverPort;

    //
    // @var     m_num_init_threads
    //
    // @type    int
    //
    // @brief   Number of threads to initialise on server startup
    //
    int m_num_init_threads;

    //
    // @var     m_serv_addr
    //
    // @type    struct sockaddr_in
    //
    // @brief   Server network information
    //
    struct sockaddr_in m_serv_addr;

    //
    // @brief   Initialises the socket and binds it to an interface(s) and port.
    //
    void initialise_server_socket();

    //
    // @brief   Prints out all sockets in m_client_sockets to the terminal (DEBUG)
    //
    void print_sockets(void);
};

}

#endif //CONNECTION_BASE_HPP
