/*
 * main.cpp
 *
 *  Created on: Mar 13, 2016
 *      Author: user
 */
#include "UdpSocketTest.h"
#include "TcpSocketTest.h"
#include "ProtocolTest.h"

using namespace std;
using namespace npl;

int main() {
	bool passFlag = true;
	UdpSocketTest udpTest;
	if (!udpTest.test()){
		cerr<<endl<<endl<<"UDP test failed"<<endl;
		passFlag = false;
	}else{
		cout<<endl<<"UDP test pass"<<endl;
	}
	cout<<endl<<endl<<endl;

	TcpSocketTest tcpTest;
	if(!tcpTest.test()){
		cerr<<endl<<endl<<"TCP test failed"<<endl;
		passFlag = false;
	}else{
		cout<<endl<<"TCP test pass"<<endl;
	}
/*
	ProtocolTest protTest;
	if(!protTest.test()){
		cerr<<endl<<endl<<"protTest test failed"<<endl;
		passFlag = false;
	}else{
		cout<<endl<<"protTest test pass"<<endl;
	}
*/


	if(passFlag){
		cout<<endl<<endl<<"**** TEST PASS ****"<<endl;
	}else{
		cerr<<endl<<endl<<"**** TEST FAIL ****"<<endl;
	}

}

