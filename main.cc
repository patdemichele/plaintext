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

void modifyRequest(char*& buf, string& host, unsigned short& port) {
  port = defaultPortNumber;
  host = splitHost(getHost(buf), port);

  buf = updateRequestLine(buf);
  string method = getMethod(buf);
  
  cout << "HOST = " << host << endl;
  cout << "METHOD = " << method << endl;
  
  cout << "Received (modified) request:" << endl << "\e[1;31m" << buf << "\e[0m";
}

int getSiteResponse(string host, unsigned short port, string req, char* resp) {
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
	
	string host;
	unsigned short port;
	int response_size;
	int clientfd;
	
    if (getClientRequest(serverfd, buf_client, clientfd) < 0) continue;
	modifyRequest(buf_client, host, port);
	if ((response_size = getSiteResponse(host, port, buf_client, buf_web)) < 0) {
	  close(clientfd);
	  continue;
	}
	sendResponse(clientfd, buf_web, response_size);

	close(clientfd);
    cout<<endl<<endl;
  }

  return 0;
}
