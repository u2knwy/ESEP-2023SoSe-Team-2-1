/*
 * EventManager.h
 *
 *  Created on: 25.05.2023
 *      Author: Maik
 */
#pragma once

#include "events.h"
#include <functional>
#include <string>
#include <map>
#include <mutex>
#include <sys/neutrino.h>
#include <sys/dispatch.h>

// qnx message declarations
typedef struct _pulse header_t;
/* Second header: used by application - if required */
typedef struct
{
	int size;  // size of data block
	int count;
	EventType eventnr;// some counter used by the application
			   // further data fields required by our application
} app_header_t;

class EventManager {
public:
	using EventCallback = std::function<void(const Event &)>;
	//typedef void (*EventCallback)(Event);
	EventManager();
	virtual ~EventManager();
	void subscribe(EventType type, EventCallback callback);
	void unsubscribe(EventType type, EventCallback callback);
	void sendEvent(const Event &event);
	int sendtoMaster(const Event &event);
	int start();
private:
	bool isMaster;
	int server_coid;
	std::map<EventType, std::vector<EventCallback>> subscribers;
	std::mutex mtx;
    void handle_app_msg(header_t hdr, int rcvid);
    void handle_ONX_IO_msg(header_t hdr, int rcvid);
    void handle_pulse(header_t hdr, int rcvid);
};
