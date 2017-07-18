#include "htmlparser.h"
#include "networking.h"
#include "readwrite.h"
#include <unistd.h>
#include <netdb.h>                // for gethostbyname
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>

// constant used for large buffers.
#define BUF_SIZE  1 << 16


using namespace std;

// sequential implementation of a simple proxy server.
int main(int argc, char* argv[]) {
  int serverfd = createServerSocket((unsigned short)12345); // magic number. for now.
  cout << "SERVER FD " << serverfd << endl;
  while (true) {
    char buf_client[BUF_SIZE]; // used to store request sent from client
    char buf_web[BUF_SIZE]; // populated when website responds to request

    // CHAPTER 1: GET REQUEST FROM CLIENT
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int proxyclientfd = accept(serverfd, (struct sockaddr *)&clientAddr, &clientAddrSize);
    if (proxyclientfd< 0) continue;
    // reads client's request
    int num_read = readall(proxyclientfd, buf_client, BUF_SIZE, "\r\n\r\n");
    // TODO: error checking
    cout << "Initially read " << num_read << " bytes."<<endl;
    cout << "Initial request " << buf_client << endl;

    // CHAPTER 2: FORWARD IT ONWARD
    unsigned short port = defaultPortNumber;
    string host = splitHost(getHost(buf_client), port);
    string path = getPath(buf_client);
    string method = getMethod(buf_client);
    cout << "HOST="<<host << endl;
    cout << "PATH="<<path << endl;
    cout << "METHOD="<<method << endl;
    //num_read += readall(proxyclientfd, buf + num_read, BUF_SIZE - num_read, "\r\n\r\n");
    string req = updateGET(buf_client, path);
    cout<<"Received (modified) request:"<<endl <<"\033[1;31m"<<req<<"\033[0m";
    int webfd = createClientSocket(host, port);
    cout << "WEBFD = " << webfd << endl;
    if (webfd < 0) {
      cout << "web client error" << endl; // this usually does not happen.
      continue;
    }
    int written=writeall(webfd, req.c_str(), req.size());
    cout << "JUST WROTE " << written << " bytes AS REQUEST TO WEBSITE"<<endl;
    cout << "Content same as written before" << endl;

    // CHAPTER 3: GET THE WEBSITE'S RESPONSE BACK
    num_read = readall(webfd, buf_web, BUF_SIZE, "\r\n\r\n");
    cout << "RECEIVED FROM WEBSITE" << num_read << " BYTES "<<endl;
    cout<<"Received response:"<<endl<<"\033[2;36m"<<buf_web<<"\033[0m";	
	
    // CHAPTER 4: WRITE RESPONSE BACK TO CLIENT
    written = writeall(proxyclientfd, buf_web, num_read);
    cout<<"Wrote "<<written<<" bytes to the browser"<<endl;

    // CHAPTER 5: CLOSE CONNECTIONS
    close(proxyclientfd);
    close(webfd);
	
    cout<<endl<<endl;
  }

  return 0;
}
