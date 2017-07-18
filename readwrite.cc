#include "readwrite.h"
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>

using namespace std;

int readall(int fd, char* buf, int max_bytes, const char* sentinel) {
  int num_read = 0;
  int sentinel_size = strlen(sentinel);
  while (num_read < max_bytes) {
    cout << "entering while loop for read"<<endl;
    int bytes = read(fd, buf + num_read, max_bytes - num_read);
    cout << "read call over"<<endl;
    if (bytes == 0) return num_read;
    if (bytes < 0) return -num_read;
    num_read += bytes;
    if (num_read >= sentinel_size && strncmp(buf + num_read - sentinel_size, sentinel, sentinel_size) == 0)
      return num_read;
    cout<<"bytes (read) = "<<bytes<<" | last four are "<<(int)buf[num_read-4]<<" and "<<(int)buf[num_read-3]<<" and "<<(int)buf[num_read-2]<<" and "<<(int)buf[num_read-1]<<endl;
  }
  return num_read;
}

int writeall(int fd, const char* buf, int num_bytes) {
  int num_written = 0;
  while (num_written < num_bytes) {
    int bytes = write(fd, buf+num_written, num_bytes - num_written);
    if (bytes < 0) return -num_written;
    num_written += bytes;
    cout<<"bytes (write)= "<<bytes<<endl;
  }
  return num_written;
}





