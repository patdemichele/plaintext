#include "htmlparser.h"
#include "networking.h"
#include <unistd.h>

#include <netdb.h>                // for gethostbyname

#include <iostream>
using namespace std;

// sequential implementation of a simple proxy server.
int main(int argc, char* argv[]) {
  int serverfd = createServerSocket((unsigned short)12345); // magic number. for now.
  cout << "SERVER FD " << serverfd << endl;
  while (true) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int connfd = accept(serverfd, (struct sockaddr *)&clientAddr, &clientAddrSize);
    char buf[1024];
    read(connfd, buf,1024);
    printf("%s", buf);
    write(connfd, "Niven is an idiot", 17);
    close(connfd);

  }
  cout << "Server is running ... just kidding" << endl;

  return 0;
}
