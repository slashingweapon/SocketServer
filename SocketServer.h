/*
 *  SocketServer.h
 *  SocketServer
 *
 *  Created by Courtney Holmes on 7/11/11.
 *  Copyright 2011 Unemployed. All rights reserved.
 *
 */

#include <list>

class SocketWorker;

class SocketServer {
public:
	SocketServer();
	~SocketServer();
	
	void* run();
	void stop() { mStop = true; }
	
	void removeWorker(SocketWorker *worker);
	void addWorker(SocketWorker *worker);

	// since the list of active workes is protected by a mutex, you would need 
	// a special function to be able to iterate through them.  Something like:
	//
	// void visitWorkers(void (*)(SocketWorker* worker));
	//
	// You would also have to work reasonably fast.  The mutex would hae to remain
	// locked for the entire iteration, and during that time no new connection threads 
	// could be started meanwhile.
	
private:
	
	pthread_mutex_t mMutex;
	bool mStop;
	int mSocket;
	
	// all variables below this line are protected by mMutex
	std::list<SocketWorker*> mWorkers;
};
