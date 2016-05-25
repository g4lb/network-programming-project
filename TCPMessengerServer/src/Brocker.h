/*
 * Brocker.h
 *
 *  Created on: May 8, 2016
 *      Author: parallels
 */

#ifndef SRC_BROCKER_H_
#define SRC_BROCKER_H_



#include "../../SocketUtils/src/MThread.h"
#include "../../SocketUtils/src/TCPSocket.h"

namespace npl {

class Brocker:public MThread {
public:
	class Handler{
	public:
		virtual void onClose(Brocker* brocker, TCPSocket* peerA, TCPSocket* peerB)=0;
	};
private:
	TCPSocket* peerA;
	TCPSocket* peerB;
	Handler* handler;
	bool active;
public:
	Brocker(Handler* handler, TCPSocket* peerA, TCPSocket* peerB);
	virtual ~Brocker();
	void run();
	void close();
};

} /* namespace npl */

#endif /* SRC_BROCKER_H_ */
