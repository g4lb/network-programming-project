/*
 * MThread.h
 *
 * this class is a wrapper to the thread object.
 * the class contains functions that enable to create, manage and stop the thread without to access the thread's functions itself
 * data members:
 * pthread_t threadId is a variable of pthread_t that keeps a thread ID after a thread is created.
 * run method: is a method that acts like a main thread.
 * start method: is the method that activate the run method, meaning the main thread.
 * waitForThread method: is a method that close the threads.
 */

#ifndef MTHREAD_H_
#define MTHREAD_H_

#include <pthread.h>

namespace npl{


class MThread{
public:
	pthread_t threadId;

public:
	virtual void run() = 0;
	void start();
	void waitForThread();
	virtual ~MThread();
};

}
#endif /* MTHREAD_H_ */
