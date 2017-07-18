#define defaultPortNumber 80
// Server utilies. createServerSocket, etc.
#include <string>

extern std::string getHost(const std::string& data);

extern std::string getPath(const std::string& data);

// in my assign7 code, I ingestPayload of the request if the method isn't HEAD
extern std::string getMethod(const std::string& header);

extern std::string updateGET(const std::string& header, const std::string& path);

extern std::string splitHost(const std::string& host, unsigned short& port);

// returns fd
extern int createClientSocket(const std::string& host, unsigned short port);

// returns listenfd, or -1.
extern int createServerSocket(unsigned short portNum);
