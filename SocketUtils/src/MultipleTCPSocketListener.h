/*
 * MultipleTCPSocketListener.h
 *
 *  this class enable to listen to multiple sockets simultaneously.
 *  data members:
 *  vector<TCPSocket*> tSockets:
 *  add method: to add a socket to the vector of sockets.
 *  the second add method:
 *  remove method: remove the socket from the vector.
 *
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