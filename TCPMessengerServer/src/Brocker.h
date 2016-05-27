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
	class BrockerHandler{
	public:
		virtual void onClose(Brocker* brocker, TCPSocket* peerA, TCPSocket* peerB)=0;
	};
private:
	TCPSocket* peerA;
	TCPSocket* peerB;
	string userNameA, userNameB;
	BrockerHandler* handler;
	bool active;
public:
	Brocker(BrockerHandler* handler, TCPSocket* peerA, TCPSocket* peerB,const string& userNameA, const string& userNameB);
	virtual ~Brocker();
	void run();
	void close();
};

} /* namespace npl */

#endif /* SRC_BROCKER_H_ */
