// Server utilies. createServerSocket, etc.
#include <string>

// establishes an endpoint with the server.
int fd_from_request(const std::string& request);
// gets the response
int get_response(int fd, const std::string& request);

void service_loop(); // incorporates these methods and other system calls to cycle and service clients
