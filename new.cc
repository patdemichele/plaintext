#include <netdb.h>
#include <iostream>

#include "networking.h"

using namespace std;

int main(int argc, char* argv[]) {

  while (true) {
	
    string host;
    cin >> host;
	/*
    struct hostent *he;
    he = gethostbyname(host.c_str());
    if (he != NULL) cout << "Success!\n";
	*/
	int sock;
	if ((sock = createClientSocket(host, 80)) != -1) {
	  cout<<"Success! ("<<sock<<")"<<endl;
	}
  }
  return 0;
}
