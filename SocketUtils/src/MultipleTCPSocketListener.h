/*
 * MultipleTCPSocketListener.h
 *
 *  Created on: Mar 20, 2016
 *      Author: user
 */

#ifndef MultipleTCPSocketListener_H_
#define MultipleTCPSocketListener_H_

#include <algorithm>
#include "TCPSocket.h"
#include <vector>

using namespace std;
namespace npl {

	class MultipleTCPSocketListener {
		typedef vector<TCPSocket*> tSockets;
		vector<TCPSocket*> sockets;

	public:
		/**
         * add socket to listen to
         */
		void add(TCPSocket* socket);
		void add(vector<TCPSocket*> sockets);
		void remove(TCPSocket* socket);
		TCPSocket* listen(int timeout);

	};

} /* namespace npl */

#endif /* MultipleTCPSocketListener_H_ */