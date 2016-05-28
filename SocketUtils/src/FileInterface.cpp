/*
 * FileInterface.cpp
 *
 *  Created on: Jan 1, 2016
 *      Author: parallels
 */

#include "FileInterface.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

using namespace npl;

void FileInterface::close(){
	::close(socket_fd);
}

ssize_t FileInterface::read(char * buffer, int length){
	return ::read(socket_fd,buffer,length);
}

int FileInterface::getSocketFd(){
	return socket_fd;
}
ssize_t FileInterface::write(const char* buff,int length){
	return ::write(socket_fd, buff,length);
}
FileInterface::~ FileInterface(){

}



