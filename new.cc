#include <netdb.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

  while (true) {
    string host;
    cin >> host;
    struct hostent *he;
    he = gethostbyname(host.c_str());
    if (he != NULL) cout << "Success!\n";

  }
  return 0;
}
