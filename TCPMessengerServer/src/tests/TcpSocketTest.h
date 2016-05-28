/*
 * TcpSocketTest.h
 *
 *  Created on: Mar 13, 2016
 *      Author: user
 */

#ifndef TCPSOCKETTEST_H_
#define TCPSOCKETTEST_H_

#include <iostream>
#include <string.h>
#include "../../../SocketUtils/src/MThread.h"

using namespace std;

namespace npl{

class TcpSocketTest: public MThread{

public:
	bool test();
	void run();
};

}
#endif /* TCPSOCKETTEST_H_ */
