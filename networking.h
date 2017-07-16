#define defaultPortNumber 80

// Server utilies. createServerSocket, etc.
#include <string>

// returns fd
int createClientSocket(const std::string& host, unsigned short port);

// returns listenfd, or -1.
int createServerSocket(unsigned short portNum);

