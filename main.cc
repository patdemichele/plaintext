#include "htmlparser.h"
#include "networking.h"


#include <netdb.h>                // for gethostbyname

#include <iostream>
using namespace std;

// sequential implementation of a simple proxy server.
int main(int argc, char* argv[]) {
  int serverfd = createServerSocket(80); // magic number. for now.
  cout << "SERVER FD " << serverfd << endl;
  while (true) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int connfd = accept(serverfd, (struct sockaddr *)&clientAddr, &clientAddrSize);
    cout << "hey I got a connection : " << connfd << endl;

    return 0;

  }
  cout << "Server is running ... just kidding" << endl;

  return 0;
}
