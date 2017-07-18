
// readwrite.h : Simple header file for 2 R/W utilities.

// reads the entire contents of the fd into buf, until encountering the sentinel.
int readall(int fd, char* buf, int max_bytes, const char* sentinel);
// writes the entire contents of the buf into fd, for the number of given bytes
int writeall(int fd, const char* buf, int num_bytes); 
