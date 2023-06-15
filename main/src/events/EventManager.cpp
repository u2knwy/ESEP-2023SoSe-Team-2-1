/*
 * EventManager.cpp
 *
 *  Created on: 25.05.2023
 *      Author: Maik
 */

#include "EventManager.h"
#include "configuration/Configuration.h"
#include "logger/logger.hpp"
#include "events.h"

#include <string>
#include <iostream>
#include <sstream>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <sys/iofunc.h>

#define ATTACH_POINT_LOCAL_M "EventMgrMaster"
#define ATTACH_POINT_LOCAL_S "EventMgrSlave"

#define ATTACH_POINT_GNS_M "/dev/name/global/EventMgrMaster"
#define ATTACH_POINT_GNS_S "/dev/name/global/EventMgrSlave"

#define STR_MSG (_IO_MAX + 1)
#define DATA_MSG (_IO_MAX + 2)


EventManager::EventManager() : server_coid(-1) {
	isMaster = Configuration::getInstance().systemIsMaster();
	if(isMaster){
		Logger::debug("EventManager is Master...");
		 if ((server_coid = name_open(ATTACH_POINT_GNS_S, NAME_FLAG_ATTACH_GLOBAL)) == -1) {
			 Logger::error("Failed Attaching to Slave EventManager");
		}
	}else{
		Logger::debug("EventManager is Slave...");
		 if ((server_coid = name_open(ATTACH_POINT_GNS_M, NAME_FLAG_ATTACH_GLOBAL)) == -1) {
				    	Logger::error("Failed Attaching to Master EventManager");
				    }

	}
}

EventManager::~EventManager() {
	name_close(server_coid);

}

void EventManager::subscribe(EventType type, EventCallback callback) {
	if (subscribers.find(type) == subscribers.end()) {
		// If event doesn't exist yet, create a new list and add it to the map
		subscribers[type] = std::vector<EventCallback>();
	}
	subscribers[type].push_back(callback);
}

void EventManager::unsubscribe(EventType type, EventCallback callback) {

}

int EventManager::sendtoMaster(const Event &ev){
	    header_t     header;
	    app_header_t app_header;
	    iov_t        iov[3]; // multi part msg

	    char  *payload1 = "intern Payload ...";

	    char  r_msg[512]; // buffer for the answe
	    	int payload_size = strlen(payload1)+1;   // +1 due to \0 at the end of a string
	    	// Compose the msg using an IOV
	    	header.type = STR_MSG;    // define msg type
	    	header.subtype = 0x00;

	    	app_header.size = payload_size;	// fill application header
	    	app_header.count = 777;
	    	app_header.eventnr = ev.type;

	    	SETIOV(iov+0, &header, sizeof(header));
	    	SETIOV(iov+1, &app_header, sizeof(app_header));
	    	SETIOV(iov+2, payload1, payload_size);

	        // send msg
	    	if (-1 == MsgSendvs(server_coid, iov, 3, r_msg, sizeof(r_msg))){
	    		Logger::error("failed sending message");
	    	    exit(EXIT_FAILURE);
	    	}
	    	// Answer form server should be structured, too.
	    	// Did not check for buffer overflow before printing r_msg

	    return EXIT_SUCCESS;
}

void EventManager::sendEvent(const Event &event) {
		if(!isMaster){
		    Logger::debug("Sending message to master");
			sendtoMaster(event);
		}
	std::stringstream ss;
	ss << "[EventManager] sendEvent: " << EVENT_TO_STRING(event.type); // << " = " << (int) event.type;

	if(event.data != -1)
		ss << ", data: " << std::to_string(event.data);

	if (subscribers.find(event.type) != subscribers.end()) {
		Logger::debug("Notifiying subscribers...");
		int i = 1;
		for(const auto& callback : subscribers[event.type]) {
			Logger::debug("Notifiying subscriber #" + std::to_string(i++));
			callback(event);
		}
	} else{
		ss << " -> No subscribers for Event!";
	}
	Logger::info(ss.str());
}
void EventManager::handle_app_msg(header_t hdr, int rcvid)
{
	Logger::debug("EventManager Master got app msg.");
	// check type
	const char *ret_msg = "Got the message r1";
	if (DATA_MSG == hdr.type)
	{
		printf("Server: DATA_MSG not supported.\n");
		MsgError(rcvid, EPERM);
		return;
	}
	if (STR_MSG == hdr.type)
	{
		printf("Server: STR_MSG received.\n");
		// read app header
		app_header_t app_header;
		MsgRead(rcvid, &app_header, sizeof(app_header), sizeof(header_t));
		// ToDo return value of MsgRead must be checked, in particular number of read bytes

		// read payload
		char *buf = (char *)malloc(app_header.size);
		// ToDo return value of malloc must be checked

		MsgRead(rcvid, buf, app_header.size, sizeof(header_t) + sizeof(app_header_t));
		// ToDo return value of MsgRead must be checked, in particular number of read bytes


		switch(app_header.eventnr){

		default:
			printf("Received EventType %s\n", EVENT_TO_STRING(app_header.eventnr));

		}


		// send reply
		MsgReply(rcvid, EOK, ret_msg, strlen(ret_msg) + 1);
		free(buf);
		return;
	}
	// Wrong msg type
	printf("Server: Wrong message type 0x%04X\n", hdr.type);
	MsgError(rcvid, EPERM);
}

void EventManager::handle_ONX_IO_msg(header_t hdr, int rcvid)
{
	Logger::debug("EventManager Master got qnx io msg.");
	if (hdr.type == _IO_CONNECT)
	{
		// QNX IO msg _IO_CONNECT was received; answer with EOK
		Logger::debug("EventManager receive IO_CONNECT.");
		MsgReply(rcvid, EOK, NULL, 0);
		return;
	}
	// Some other QNX IO message was received; reject it
	Logger::debug("EventManager Master got unexpected IO Message.");
	MsgError(rcvid, ENOSYS);
}

void EventManager::handle_pulse(header_t hdr, int rcvid)
{
	Logger::debug("EventManager Master got pulse msg.");
	switch (hdr.code)
	{
	case _PULSE_CODE_DISCONNECT:
		Logger::debug("Server received _PULSE_CODE_DISCONNECT...");
		/* A client disconnected all its connections (called
		 * name_close() for each name_open() of our name) or
		 * terminated. */
		ConnectDetach(hdr.scoid);
		break;
	case _PULSE_CODE_UNBLOCK:
		Logger::debug("Server received _PULSE_CODE_UNBLOCK...");
		/* REPLY blocked client wants to unblock (was hit by
		 * a signal or timed out).  It's up to you if you
		 * reply now or later. */
		break;
	default:
		/* A pulse sent by one of your processes or a
		 * _PULSE_CODE_COIDDEATH or _PULSE_CODE_THREADDEATH
		 * from the kernel? */
		Logger::debug("Server received some pulse..");
		break;
	}
}
int EventManager::start() {
	Logger::debug("EventManager Start Loop.");
		name_attach_t *attach;
		// Create a global name (/dev/name/global/...) known within the network

		if (isMaster && ((attach = name_attach(NULL, ATTACH_POINT_LOCAL_M, NAME_FLAG_ATTACH_GLOBAL)) == NULL))
		{
			perror("Master: name_attach failed. Check GNS status.");
			return EXIT_FAILURE;
		}

		if (!isMaster && ((attach = name_attach(NULL, ATTACH_POINT_LOCAL_S, NAME_FLAG_ATTACH_GLOBAL)) == NULL))
				{
						perror("Slave: name_attach failed. Check GNS status.");
						return EXIT_FAILURE;
				}




		// Do your MsgReceive's here now with the chid
		while (1)
		{
			// Waiting for a message and read first header
			header_t header;
			int rcvid = MsgReceive(attach->chid, &header, sizeof(header_t), NULL);

			if (rcvid == -1)
			{ // Error occurred
				perror("Server: MsgReceived failed");
				break;
			}

			if (rcvid == 0)
			{ // Pulse was received
				handle_pulse(header, rcvid);
				continue;
			}
			// some sync msg was received
			if ((_IO_BASE <= header.type) && (header.type <= _IO_MAX))
			{
				// Some QNX IO msg generated by gns was received
				handle_ONX_IO_msg(header, rcvid);
				continue;
			}
			//  A sync msg (presumable ours) was received; handle it
			handle_app_msg(header, rcvid);
		}

		// Server stopped; remove the name from the space
		name_detach(attach, 0);

		return EXIT_SUCCESS;
}
