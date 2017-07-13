// server utility methods here
#include "server.h"
#include <iostream>
#include <netdb.h>                // for gethostbyname                                         
#include <sys/socket.h>           // for socket, AF_INET                                       
#include <sys/types.h>            // for SOCK_STREAM                                           
#include <unistd.h>               // for close                                                 
#include <cstring>                // for memset                                                

#include <cstring>
#include <sstream>
#include <string>
#include <climits>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <getopt.h>
#include <unistd.h>


using namespace std;
void example_method(int a) {
  cout << a<< endl;
}


int establishClientSocket(const string& host, unsigned short port) {
  int error;
  char buffer[1 << 16]; // make it huge                                                        
  struct hostent entry, *he;
  gethostbyname_r(host.c_str(), &entry, buffer, sizeof(buffer), &he, &error);
  if (he == NULL) return kClientErr;

  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0) return kClientErr;

  struct sockaddr_in serverAddress;
  memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  serverAddress.sin_addr.s_addr = ((struct in_addr *)he->h_addr)->s_addr;

  if (connect(s, (struct sockaddr *) &serverAddress,
              sizeof(serverAddress)) != 0) {
    close(s);
    return kClientErr;
  }
  return s;
}


int establishServerSocket(int portNumber) {
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) {
    return -1;
  }
  const int optval = 1;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval , sizeof(int));

  struct sockaddr_in serverAddr;
  bzero(&serverAddr, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddr.sin_port = htons(portNumber);
  struct sockaddr *sa = (struct sockaddr *) &serverAddr;

  // ::bind needed to be clear that global bind in the C libs is                               
  // being called instead of the std::bind from C++'s STL.                                     
  if (::bind(listenfd, sa, sizeof(serverAddr)) < 0) {
    ostringstream oss;
    oss << "Failed to associate listening socket with port " << portNumber << ".";
    return -1;
  }

  const size_t kMaxQueuedRequests = 128; // 128 is the largest allowed value                   
  if (listen(listenfd, kMaxQueuedRequests) < 0) {
    return -1;
  }
  return listenfd;

}

