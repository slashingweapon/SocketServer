#include <signal.h>
#include "SocketServer.h"

static SocketServer *gServer = NULL;

/*	Provide a mechanism for a graceful shutdown, using SIGINT or SIGHUP. */
void handle_HUP(int x) {
	if (gServer)
		gServer->stop();
}

int main (int argc, char * const * argv[]) {
	/*	Set up our signal handlers */
	struct sigaction actionInfo;
	struct sigaction oldActionInfo;
	
	bzero(&actionInfo, sizeof(actionInfo));
	actionInfo.sa_handler = handle_HUP;
	
	sigaction(SIGHUP, &actionInfo, &oldActionInfo);
	sigaction(SIGINT, &actionInfo, &oldActionInfo);
	
	/*	Create and run the server. */
	gServer = new SocketServer();
	
	gServer->run();
	
	delete gServer;
	
	return 0;
}
