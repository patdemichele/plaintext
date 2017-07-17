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
  size_t end = data.find("\n", begin);
  return data.substr(begin, end-begin);
}


// establishes an endpoint with the server.                                                     
//client connects to a server.
int createClientSocket(const string& host, unsigned short port) {
  struct hostent *he;
  he = gethostbyname(host.c_str());
  if (he == NULL) {
	herror("gethostbyname");
	cout <<"failure of gethostbyname when called on "<< host.c_str()<<endl;
	return -1;
  }

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) return -1;

  struct sockaddr_in serverAddress;
  memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  serverAddress.sin_addr.s_addr = ((struct in_addr *)he->h_addr)->s_addr;

  if (connect(sock, (struct sockaddr *) &serverAddress, 
	      sizeof(serverAddress)) != 0) {
    close(sock);
    return -1;
  }
  return sock;
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

