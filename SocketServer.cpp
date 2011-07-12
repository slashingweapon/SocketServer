/*
 *  SocketServer.cpp
 *  SocketServer
 *
 *  Created by Courtney Holmes on 7/11/11.
 *  Copyright 2011 Unemployed. All rights reserved.
 *
 */

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include "SocketServer.h"
#include "SocketWorker.h"

/*	Server setup is the usual stuff:
		create a socket
		bind to an address
		listen
 */
SocketServer::SocketServer() {
	struct sockaddr_in addr;
	int isOn = 1;
	
	pthread_mutex_init(&mMutex, 0);
	mStop = false;
	
	mSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mSocket < 0) {
		printf("Could not create the socket.\n");
		mStop = true;
	}
	
	if (!mStop && setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &isOn, sizeof(isOn)) != 0) {
		printf("Could not set reuse-address socket option\n");
		mStop = true;
	}
	
	if (!mStop) {		
		bzero(&addr, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(3137);
	
		if (bind(mSocket, (struct sockaddr*) &addr, sizeof(addr)) != 0) {
			printf("Could not bind to server port.\n");
			mStop = true;
		}
	}	
	
	if (!mStop && listen(mSocket, 100) != 0) {
		printf("Could not listen on socket.\n");
		mStop = true;
	}
}

SocketServer::~SocketServer() {
	if (mSocket >= 0)
		close(mSocket);
}

/*	For as long as we aren't told to stop, we're going to keep accepting 
	new connections.  Each connection gets its own worker, which is
	added to our list of workers.  Then the worker is started.
 
	Repeat.
 */
void* SocketServer::run() {
	int peer;
	struct sockaddr peerAddress;
	socklen_t peerAddressLength;
	
	while (!mStop) {
		peerAddressLength = sizeof(peerAddress);
		peer = accept(mSocket, &peerAddress, &peerAddressLength);
		if (peer >= 0) {
			SocketWorker *worker = new SocketWorker(peer, this);
			
			this->addWorker(worker);
			worker->start();
		}
	}
	return NULL;
}

/*	To keep a valid list of workers accessible by anyone, we have to 
	synchronize it.
 */

void SocketServer::addWorker(SocketWorker *worker) {
	if (!pthread_mutex_lock(&mMutex)) {
		mWorkers.push_front(worker);
		pthread_mutex_unlock(&mMutex);
	}		
}

void SocketServer::removeWorker(SocketWorker *worker) {
	std::list<SocketWorker*>::iterator iter;
	
	if (!pthread_mutex_lock(&mMutex)) {
		iter = find(mWorkers.begin(), mWorkers.end(), worker);
		if (iter != mWorkers.end())
			mWorkers.erase(iter);
		pthread_mutex_unlock(&mMutex);
	}
}


