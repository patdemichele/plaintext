#define defaultPortNumber 80

// Server utilies. createServerSocket, etc.
#include <string>
int createClientSocket(const std::string& host, unsigned short port);


void service_loop(); // incorporates these methods and other system calls to cycle and service clients
