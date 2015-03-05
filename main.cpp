#include <iostream>
#include <string>
#include <map>

#include "connection/httpconnection.hpp"

#include "protocols/http/requesteventhandler.hpp"

#include "utility/threading/threadpool.hpp"

using namespace connection;

void work_to_do()
{
    std::cout << "Doing some work!" << std::endl;
}

int main(void)
{
	std::cout << "Starting proxy..." << std::endl;

	//HttpConnection cb;

	//cb.server_listen();

	ThreadPool tp(10);
	tp.add_work(work_to_do);

	//std::string str("GET / HTTP/1.1\r\nhost: www.play.com\r\n Lang: crap\nnaff: this id crap\nThis header:    dsfjbfsdjb sdfkjbskjbfg\n");

	//RequestEventHandler rp;

	//rp.parse(str);

	//HttpRequest req = rp.get_bttp_request();

	/*std::cout << "Request object: " << req.get_request_method() << " " << req.get_request_resource() << " " << req.get_request_version() << std::endl;

	std::map<std::string, std::string> headers_map = req.get_headers();

	std::map<std::string, std::string>::iterator it = headers_map.begin();

	for(; it != headers_map.end(); ++it)
	{
	    std::cout << "Header: " << it->first << " " << it->second << std::endl;
	}*/

	//std::cout << req.get_header() << std::endl;

	return 0;
}
