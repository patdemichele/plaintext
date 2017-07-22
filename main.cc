#include "htmlparser.h"
#include "networking.h"
#include "readwrite.h"
#include <unistd.h>
#include <netdb.h>                // for gethostbyname
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>

#define BUF_SIZE  (1 << 16)
#define SERVER_PORT 12345

using namespace std;
int getClientRequest(int serverfd, char* buf, int& clientfd) {
  struct sockaddr_in clientAddr;
  socklen_t clientAddrSize = sizeof(clientAddr);
  
  clientfd = accept(serverfd, (struct sockaddr *)&clientAddr, &clientAddrSize);
  if (clientfd < 0) return -1;
  
  // reads client's request
  int num_read = readall(clientfd, buf, BUF_SIZE, "\r\n\r\n");
 
  cout << "Initially read " << num_read << " bytes."<<endl;
  cout << "Initial request " << endl << "\e[33m" << buf << "\e[0m" << endl;

  return num_read;
}

int getSiteResponse(string req, char* resp) {
  unsigned short port = defaultPortNumber;
  string host = splitHost(getHost(req), port);
  string method = getMethod(req);

  if (host.find("firefox") != string::npos) return -1;
  
  cout << "HOST = " << host << endl;
  cout << "METHOD = " << method << endl;
  
  int webfd = createClientSocket(host, port);
  cout << "WEBFD = " << webfd << endl;
  if (webfd < 0) {
	cout << "web client error" << endl;
	return -1;
  }
  
  int num_written = writeall(webfd, req.c_str(), req.size());
  cout << "Wrote " << num_written << " bytes as request to website" << endl;

  int num_read = readall(webfd, resp, BUF_SIZE, "\r\n\r\n");
  cout << "received " << num_read << " bytes from website" << endl;
  cout <<"Received response:" << endl << "\e[2;36m" << resp << "\e[0m";

  close(webfd);
  return num_read;
}

int sendResponse(int clientfd, char* resp, int size) {
  int num_written = writeall(clientfd, resp, size);
  cout << "Wrote " << num_written << " bytes back to the browser" << endl;
  return num_written;
}

// sequential implementation of a simple proxy server.
int main(int argc, char* argv[]) {
  int serverfd = createServerSocket(SERVER_PORT);
  cout << "SERVER FD " << serverfd << endl;
  cout << "Starting server at localhost: " << SERVER_PORT << endl;
  while (true) {
    char buf_client_arr[BUF_SIZE]; // used to store request sent from client
    char buf_web[BUF_SIZE]; // populated when website responds to request

	// Need ability to change address pointed to
	// address will only move forward so no fear of accessing unowned memory
	char* buf_client = buf_client_arr;
    
	int response_size;
	int clientfd;
	
    if (getClientRequest(serverfd, buf_client, clientfd) < 0) {
	  cout << "Failed to get client request" << endl;
	  continue;
	}
	buf_client = updateRequestLine(buf_client);
  
	cout << "Received (modified) request:" << endl << "\e[1;31m" << buf_client << "\e[0m";

	if ((response_size = getSiteResponse(buf_client, buf_web)) >= 0) {
	  sendResponse(clientfd, buf_web, response_size);
	} else {
	  cout << "Failed to get site response" << endl;
	}

	close(clientfd);
    cout<<endl<<endl;
  }

  return 0;
}
