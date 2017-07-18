// server utility methods here
#include "networking.h"
#include <netdb.h>                // for gethostbyname
#include <sys/socket.h>           // for socket, AF_INET
#include <sys/types.h>            // for SOCK_STREAM
#include <unistd.h>               // for close                                 
#include <cstring>                // for memset

#include <arpa/inet.h>             // for htonl, htons, etc.
#include <fcntl.h>

#include <string> // for c++ string
#include <iostream>
using namespace std;


string getHost(const string& data) {

  string look_for = "Host: ";
  size_t begin = data.find(look_for) + look_for.size();
  size_t end = min(data.find("\n", begin), data.find(":",begin)+1);
  return  data.substr(begin, end-begin-1);
}

string getPath(const string& header) {
  static const string needle = "http://";
  
  size_t begin = header.find("GET");
  begin = header.find(needle, begin) + needle.size();

  begin = header.find("/", begin);
  size_t end = header.find(" ", begin);

  return header.substr(begin, end-begin);
}

string updateGET(const string& header, const string& path) {
  size_t begin = header.find("GET") + 4;
  size_t end = header.find(" ", begin);

  string ret = header;
  return ret.replace(begin, end-begin, path);
}

// establishes an endpoint with the server.                                                     
//client connects to a server.
int createClientSocket(const string& host, unsigned short port) {
  struct addrinfo *res;
  int err = getaddrinfo(host.c_str(), to_string(port).c_str(), NULL, &res);
  if (err != 0) {
	cout<<"getaddrinfo failed on input "<<host.c_str()<<endl
	    <<"\terror: "<<gai_strerror(err)<<endl;
	return -1;
  }

  for (struct addrinfo* curr = res; curr != NULL; curr = curr->ai_next) {
	int sock = socket(curr->ai_family, curr->ai_socktype, curr->ai_protocol);
	if (sock != -1 && connect(sock, curr->ai_addr, curr->ai_addrlen) != -1) return sock;
	close(sock);
  }
  return -1;
}
//host makes itself a server on portNum.
int createServerSocket(unsigned short portNum) {
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) { cout << "listenfd initially -1\n"; return -1;}

  const int optval = 1;
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval , sizeof(int)) < 0){ close(listenfd); cout <<"setsock error \n"; return -1; }

  struct sockaddr_in serverAddr;
  bzero(&serverAddr, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddr.sin_port = htons(portNum);
  struct sockaddr *sa = (struct sockaddr *) &serverAddr;

  if (::bind(listenfd, sa, sizeof(serverAddr)) < 0) {
    close(listenfd);
    cout << "bind fail " << endl;
    return -1;
  }

  const size_t kMaxQueuedRequests = 128;
  if (listen(listenfd, kMaxQueuedRequests) < 0){ cout << "listen error\n"; close(listenfd); return -1; }
  
  return listenfd;
}

