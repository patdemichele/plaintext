// server utility methods here
#include "networking.h"
#include "readwrite.h"
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
  size_t end = data.find("\n", begin);
  return  data.substr(begin, end-begin-1);
}

string getMethod(const string& header) {
  return header.substr(0, header.find(" "));
}

char* updateRequestLine(char* header) {
  char* beg = strstr(header, " ") + 1;
  char* end = strstr(beg, " ");
  
  char* pbeg = end;
  while (*(--pbeg) != '/');

  size_t method_size = beg - header;
  memcpy(pbeg - method_size, header, method_size);

  return pbeg - method_size;
}

string splitHost(const string& host, unsigned short& port) {
  size_t pos = host.find(":");
  if (pos != string::npos) {
	port = atoi(host.substr(pos+1).c_str());
	return host.substr(0, pos);
  }
  return host;
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

