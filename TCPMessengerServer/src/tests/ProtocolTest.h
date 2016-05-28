/*
 * ProtocolTest.h
 *
 *  Created on: May 1, 2016
 *      Author: user
 */

#ifndef PROTOCOLTEST_H_
#define PROTOCOLTEST_H_

#include "../../../SocketUtils/src/MThread.h"
#include "../../../SocketUtils/src/TCPMessengerProtocol.h"

namespace npl {

class ProtocolTest : public MThread {
public:
	ProtocolTest();
	virtual ~ProtocolTest();

	bool test();
	void run();
};

} /* namespace npl */

#endif /* PROTOCOLTEST_H_ */
