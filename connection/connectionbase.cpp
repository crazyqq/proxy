
#include "connectionbase.hpp"

#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/tcp.h>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <unistd.h>

//Specify the connection queue
//Some OS's limit this to 5
#define CONNECTIONQUEUE 5

//Client buffer size for received messages
#define CLIENTBUFFERSIZE 1024

#define DEFAULTPORT 8080

using namespace std;

namespace connection
{

//
// Default Constructor
//
ConnectionBase::ConnectionBase():
    m_server_sock(0),
    m_serverPort(DEFAULTPORT),
    m_num_init_threads(5)
{
    initialise_server_socket();
}

//
// Constructor
//
ConnectionBase::ConnectionBase(Uint16 port):
    m_server_sock(0),
    m_num_init_threads(5)
{
    m_serverPort = port;

    initialise_server_socket();
}

//
// Constructor
//
ConnectionBase::ConnectionBase(Uint16 port, Uint16 number_of_threads):
    m_server_sock(0)
{
    m_serverPort = port;
    m_num_init_threads = number_of_threads;

    initialise_server_socket();
}


//
// Destructor
//
ConnectionBase::~ConnectionBase()
{
	//CLose all client sockets
	if(m_client_sockets.begin() != m_client_sockets.end())
	{
		close_all_connections();
	}

    //Close the main listening socket
    if (m_server_sock)
    {
        close(m_server_sock);
    }
}

//
// Initialises the socket and binds it to an interface(s) and port
//
void ConnectionBase::initialise_server_socket()
{
    //Used to output return status of calls for debug purposes.
    int return_status = 0;

    //Create a TCP socket
    //Returns -1 if something went wrong
    m_server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    int val = 1;
    //Set reuse address on the socket options.
    //If this returns < 0 then there was a problem.
    return_status = setsockopt(m_server_sock, SOL_SOCKET, SO_REUSEADDR,
                                          &val, sizeof(val));

    if (return_status < 0)
    {
        cout << "Error setting socket options. Error status: " << return_status << endl;
    }

    // Clear the server memory
    memset(&m_serv_addr, 0, sizeof(m_serv_addr));

    m_serv_addr.sin_family = AF_INET;

    // Bind to all addresses
    m_serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Set the port
    m_serv_addr.sin_port = htons(m_serverPort);

    // Bind to address and port using server socket
    (__bind<int &, sockaddr *, unsigned long>)return_status = bind(m_server_sock,
                            (struct sockaddr *) &m_serv_addr,
                            sizeof(m_serv_addr));

    //Check if the bind was a success
    if (return_status != 0)
    {
        cout << "Problem binding to socket" << endl;
        close(m_server_sock);
    }
}

//
// Creates a threadpool of listening threads
//
bool ConnectionBase::server_listen()
{
    //Counter for thread loop
    int threadCount = 0;
    //Variable for return results
    int result = 0;

    //Initialise a thread ID
    pthread_t thread_id;

    //Set the socket to listen
    result = listen(m_server_sock, CONNECTIONQUEUE);
    if(result == -1)
    {
        cout << "Problem listening on server socket" << endl;
        close(m_server_sock);
        return false;
    }

    //Loop to create the thread pool based on the number declared in
    //m_num_init_threads
    for(threadCount = 0; threadCount < m_num_init_threads; ++threadCount)
    {
        //Create the child thread
        result = pthread_create(&thread_id, NULL, server_thread_helper, this);

        //Check if the thread creation was successful
        if (result != 0)
        {
            cout << "Could not create thread" << endl;
            continue;
        }

        //Allows the child thread to start up by giving up the remainder
        //of the parents allotted time slice
        sched_yield();
    }

    //Causes the server to wait until the child threads have completed
    pthread_join (thread_id, NULL);

    return true;
}

//
// Send a message to a specific client
//
bool ConnectionBase::send_message_to_client(const string &message, const int &socket)
{
    return write(socket, message.c_str(), strlen(message.c_str()));
}

//
// Server thread which listens for incoming client connections
//
void* ConnectionBase::server_thread(void)
{
    //Server listening socket descriptor
    int listen_socket = m_server_sock;
    //Client socket
    int client_socket = 0;
    //Message buffer with one extra for null terminator
    char client_buffer[CLIENTBUFFERSIZE + 1];
    //Byte read count
    int num_read = 0;

    cout << "Child thread " << pthread_self() << " created!" << endl;
    cout.flush();

    while (true)
    {
        //Wait for a client connection
        client_socket = accept(listen_socket, NULL, NULL);

        cout << "Client connected to thread " << pthread_self() << endl;

        int val = 1;

        //Set the socket to TCP_NODELAY so messages aren't buffered
        int return_status = setsockopt(client_socket, IPPROTO_TCP, TCP_NODELAY,
                                          &val, sizeof(val));

        //Check if setting the socket options was successful
        if (return_status < 0)
        {
            cout << "Error setting socket options. Error status: " << return_status << endl;
            break;
        }


        //Add the client socket to the socket vector
        m_client_sockets.push_back(client_socket);

        //Keep processing messages while the client is connected
        while (find (m_client_sockets.begin(), m_client_sockets.end(), client_socket) != m_client_sockets.end())
        {
            //Read from the client socket into our buffer
            num_read = recv(client_socket, client_buffer, CLIENTBUFFERSIZE, 0);

            //If we actually received something
            if ( num_read > 0 )
            {
                //Terminate the message with a null character
                client_buffer[num_read] = '\0';

                //Process the client message
                process_request(client_buffer, client_socket);
            }
        }

        //Reset the client socket
        client_socket = 0;

        cout << "Client in thread " << pthread_self() << " exited!" << endl;
        cout.flush();
    }

    return 0;
}

//
// This is a helper thread which simply calls server_thread()
//
void* ConnectionBase::server_thread_helper(void *my_instance)
{
    return ((ConnectionBase*)my_instance)->server_thread();
}

//
// Closes the socket passed in
//
bool ConnectionBase::close_connection(int &socket)
{
    bool result = true;

    //Close the client socket
    if( close(socket) == 0 )
    {
        //Remove the socket from the socket vector
        m_client_sockets.erase(remove(m_client_sockets.begin(),
                            m_client_sockets.end(), socket),
                            m_client_sockets.end());

        socket = 0;
    }
    else
    {
        result = false;
    }

    //DEBUG OUTPUT
    print_sockets();

    return result;
}

//
// Closes all active connections to the server
//
bool ConnectionBase::close_all_connections()
{
    //TODO: fix
    bool result = true;

    //Loop through and call a close on each connection
    vector<int>::iterator it;

    //Call close on each socket descriptor in the vector
    for(it=m_client_sockets.begin(); it < m_client_sockets.end(); ++it)
    {
        close_connection(*it);
    }

    return result;
}

//
// Prints out all sockets in m_client_sockets to the terminal
//
void ConnectionBase::print_sockets(void)
{
    cout << "Socket vector contains: " << endl;

    vector<int>::iterator it;

    //Display the vector containing all sockets connected
    for(it = m_client_sockets.begin(); it < m_client_sockets.end(); ++it)
    {
        cout << *it << endl;
    }
}

}
