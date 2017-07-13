// Server utilies. createServerSocket, etc.
#include <string>
void example_method(int a);

const int kClientErr = -1;

int establishClientSocket(const std::string& host, unsigned short port);
int establishServerSocket(int port);
