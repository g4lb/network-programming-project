/*
 * MultipleTCPSocketListener.cpp
 *
 *  Created on: Mar 20, 2016
 *      Author: user
 */

#include "MultipleTCPSocketListener.h"

using namespace npl;
	void MultipleTCPSocketListener::add(TCPSocket* socket){
		sockets.push_back(socket);

	}

	void MultipleTCPSocketListener::remove(TCPSocket *socket) {

		sockets.erase(std::remove(sockets.begin(),sockets.end(), socket),sockets.end());


	}

	void MultipleTCPSocketListener::add(vector<TCPSocket*> sockets){
		this->sockets = sockets;
	}

	TCPSocket* MultipleTCPSocketListener::listen(int timeout){
		fd_set set;
		int nfd = 0;
		FD_ZERO(&set);
		tSockets::iterator iter = sockets.begin();
		for(;iter!= sockets.end();iter++){
			TCPSocket* sock = *iter;
			int fd = sock->getSocketFd();
			FD_SET(fd,&set);
			if (fd >= nfd){
				nfd = fd+1;
			}
		}

		int rc;
		if(timeout > 0){
			struct timeval timet;
			timet.tv_sec = timeout;
			timet.tv_usec = 0;
			rc = select(nfd, &set,NULL,NULL,&timet);
		}else{
			rc = select(nfd, &set,NULL,NULL,NULL);
		}
		if (rc<1){
			FD_ZERO(&set);
			return NULL;
		}

        cout << "someone sent a msg"<<endl;
		iter = sockets.begin();
		for(;iter!= sockets.end();iter++){
			TCPSocket* sock = *iter;
            cout << "checking if socket "<<sock->fromAddr()<<" sent the msg"<<endl;
			int fd = sock->getSocketFd();

            //*******
            fd_set setInner;
            FD_ZERO(&setInner);
            FD_SET(fd,&setInner);
            struct timeval timet;
            timet.tv_sec = 0;
            timet.tv_usec = 5;
            if (select(fd, &setInner,NULL,NULL,&timet)>0)
            {
                FD_ZERO(&set);
                FD_ZERO(&setInner);
                return sock;
            }
            //*******
		}
		FD_ZERO(&set);
		return NULL;
	}