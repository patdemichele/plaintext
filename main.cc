#include "htmlparser.h"
#include "networking.h"
#include <unistd.h>

#include <netdb.h>                // for gethostbyname

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>

using namespace std;

int readall(int fd, char* buf, int max_bytes, const char* sentinel) {
  int num_read = 0;
  int sentinel_size = strlen(sentinel);
  while (num_read < max_bytes) {
    int bytes = read(fd, buf + num_read, max_bytes - num_read);
    if (bytes == 0) return num_read;
    if (bytes < 0) return -num_read;
    num_read += bytes;
    if (num_read >= sentinel_size && strcmp(buf + num_read - sentinel_size, sentinel) == 0) return num_read;
  }
  return num_read;
}

int writeall(int fd, const char* buf, int num_bytes) {
  int num_written = 0;

  while (num_written < num_bytes) {
    int bytes = write(fd, buf+num_written, num_bytes - num_written);
    if (bytes < 0) return -num_written;
    num_written += bytes;
  }

  return num_written;

}




// sequential implementation of a simple proxy server.
int main(int argc, char* argv[]) {
  static const int BUF_SIZE = 1 << 16;
  
  int serverfd = createServerSocket((unsigned short)12345); // magic number. for now.

  cout << "SERVER FD " << serverfd << endl;
  while (true) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int connfd = accept(serverfd, (struct sockaddr *)&clientAddr, &clientAddrSize);
    if (connfd < 0) continue;

    char buf[BUF_SIZE];

    int num_read = readall(connfd, buf, BUF_SIZE, "\r\n\r\n");
    // TODO: error checking
    cout << "Initially read " << num_read << " bytes."<<endl;
    cout << "Initial request " << buf << endl;
    string host = getHost(buf);
    string path = getPath(buf);
    cout << "HOST="<<host << endl;
    cout << "PATH="<<path << endl;

    string req = updateGET(buf, path);

    cout<<"Received (modified) request:"<<endl <<"\033[1;31m"<<req<<"\033[0m";
    //printf("%s", buf);
	
    int clientfd = createClientSocket(host, defaultPortNumber);
    cout << "CLIENTFD = " << clientfd << endl;
    if (clientfd < 0) {
      cout << "client error" << endl;
      continue;
    }
	
    int written=writeall(clientfd, req.c_str(), req.size());
    cout << "JUST WROTE " << written << " bytes AS REQUEST TO WEBSITE"<<endl;
    cout << "Content: " << req << endl;
    num_read = readall(clientfd, buf, BUF_SIZE, "\r\n\r\n");
	cout << "RECEIVED FROM WEBSITE" << num_read << " BYTES "<<endl;
	cout<<"Received response:"<<endl<<"\033[2;36m"<<buf<<"\033[0m";	
	
    writeall(connfd, buf, num_read);
    close(connfd);
	
    cout<<endl<<endl;
  }
  cout << "Server is running ... just kidding" << endl;

  return 0;
}
