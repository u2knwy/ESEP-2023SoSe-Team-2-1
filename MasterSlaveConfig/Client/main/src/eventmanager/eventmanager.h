/*
 * eventmanager.h
 *
 *  Created on: 10.05.2023
 *      Author: domxs
 */

#ifndef SRC_EVENTMANAGER_EVENTMANAGER_H_
#define SRC_EVENTMANAGER_EVENTMANAGER_H_

#include "../events/eventtypes_enum.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <sys/iofunc.h>


#define NO_OF_MSGS			6
#define ATTACH_POINT 		"/dev/name/global/myservice"

/* Fist header used by GNS and application */
typedef struct _pulse header_t;

/* header types
 *  0 .. _IO_BASE -1 						: Pulse Messages
 * 	_IO_BASE = 0x100 .. _IO_MAX = 0x1FF 	: Sync Messages of the System
 * 	_IO_MAX + 1 ..							: Sync Messages of our application
 */

// Messages types of our application
#define STR_MSG   (_IO_MAX + 1)
#define DATA_MSG  (_IO_MAX + 2)

/* Second header: used by application - if required */
typedef struct {
	int size; // size of data block
	int count;
	EventType eventnr;// some counter used by the application
	// further data fields required by our application
} app_header_t;


class eventmanager {
private:
	 int server_coid;
public:
	eventmanager(){
		// Use GNS for setting up the connection to the server - running somewhere in the network
		    if ((server_coid = name_open(ATTACH_POINT, NAME_FLAG_ATTACH_GLOBAL)) == -1) {
		    	perror("Client: name_open failed, check server");
		    }
		    printf("nameOpened EventMgr, channelid %i\n", server_coid);

	};
	virtual ~eventmanager(){
		printf("destroying eventMgr \n");
	    // close connection
	    name_close(server_coid);};
	int sendMessagePayload(EventType event, char* payload);
};

#endif /* SRC_EVENTMANAGER_EVENTMANAGER_H_ */
