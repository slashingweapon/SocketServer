/*
 *  SocketWorker.h
 *  SocketServer
 *
 *  Created by Courtney Holmes on 7/11/11.
 *  Copyright 2011 Unemployed. All rights reserved.
 *
 */

#include <pthread.h>

class SocketServer;

/*	So, the right way to do this is to have a usable thread class, and
	then just have this class implement the equivalent of Java's Runnable
	interface.
 
	Even better, if you have a JavaScript-style future() or an Apple-style
	dispatch_async() function, use that.
 */
class SocketWorker {
public:
	SocketWorker(int socket, SocketServer *server);
	~SocketWorker();
	
	/*	Use to create a thread and invoke run() */
	void start();
	
	/*	Called from the executing thread. */
	void* run();
	
private:
	int mSocket;
	SocketServer *mServer;
	pthread_t mThread;
};
