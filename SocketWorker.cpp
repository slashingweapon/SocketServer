/*
 *  SocketWorker.cpp
 *  SocketServer
 *
 *  Created by Courtney Holmes on 7/11/11.
 *  Copyright 2011 Unemployed. All rights reserved.
 *
 */

#include <sys/socket.h>
#include <unistd.h>
#include "SocketWorker.h"
#include "SocketServer.h"

/*	The usual posix-compliant thread entry function */
static void * runObject(void *arg) {
	void *retval = NULL;
	SocketWorker *theWorker = reinterpret_cast<SocketWorker*> (arg);
	
	if (theWorker)
		retval = theWorker->run();
	
	delete theWorker;
	
	return retval;
}


SocketWorker::SocketWorker(int socket, SocketServer *server) {
	mSocket = socket;
	mServer = server;
	mThread = 0;
}

SocketWorker::~SocketWorker() {

}

void SocketWorker::start() {
	pthread_create(&mThread, NULL, runObject, this);
}

void* SocketWorker::run() {
	pthread_detach(pthread_self());
	write(mSocket, "\nHello, client.\n\n", 17);
	close(mSocket);
	mServer->removeWorker(this);
	
	return NULL;
}

