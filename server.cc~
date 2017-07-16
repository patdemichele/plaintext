// server utility methods here
#include "server.h"
#include <netdb.h>                // for gethostbyname
#include <sys/socket.h>           // for socket, AF_INET
#include <sys/types.h>            // for SOCK_STREAM
#include <unistd.h>               // for close                                 
#include <cstring>                // for memset

#include <string> // for c++ string

using namespace std;

// establishes an endpoint with the server.                                                     
//client connects to a server.
int createClientSocket(const string& host, unsigned short port) {

  struct hostent *he;
  he = gethostbyname(host.c_str());
  if (he == NULL) return -1;

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


void service_loop() {
  printf("to be implemented");
}                                                                                         


