/*
 * FileInterface.h
 *
 *  this is an interface that the class socket inherited from him.
 *  data members:
 *  int socket_fd holds the number of the socket.
 *  close method: the method close the socket.
 *  read method: the method read information from the socket.
 *  write method: the method write into the socket.
 *  getSocketFd method: the method returns the number of the socket.
 */

#ifndef FILEINTERFACE_H_
#define FILEINTERFACE_H_

#include <netinet/in.h>
#include <inttypes.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>

using namespace std;

namespace npl {

class FileInterface {
public:
	int socket_fd;
	virtual void close();
	virtual~ FileInterface();
	virtual ssize_t read(char * buffer, int length);
	virtual ssize_t write(const char* buff,int length);
	virtual int getSocketFd();
};

} /* namespace npl */

#endif /* FILEINTERFACE_H_ */
