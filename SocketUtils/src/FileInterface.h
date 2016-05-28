/*
 * FileInterface.h
 *
 *  Created on: Jan 1, 2016
 *      Author: parallels
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
