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


EventManager::EventManager() : server_coid(-1) {
	isMaster = Configuration::getInstance().systemIsMaster();
	connectGNS();
}

EventManager::~EventManager() {
	disconnectGNS();
}

void EventManager::connectGNS() {
	if(isMaster) {
		// create service
		if( (attachedServer = name_attach(NULL, "SERVICE", NAME_FLAG_ATTACH_GLOBAL)) == NULL) {
			Logger::error("name_attach failed");
		}
		Logger::info("Master created GNS Service");
	} else {
		// connect to master service
		if( (server_coid = name_open("SERVICE", NAME_FLAG_ATTACH_GLOBAL)) == -1 ) {
			throw runtime_error("failed to connect to service");
		}
		Logger::info("Slave attached to GNS Service @Master");
	}
}

void EventManager::disconnectGNS() {
	if(isMaster) {
		// terminate service
		if( name_detach(attachedServer, 0) == -1 ) {
			Logger::error("failed detaching server");
		}
	} else {
		// disconnect from master
		if( name_close(server_coid) == -1 ) {
			Logger::error("failed detaching client");
		}
	}
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

void EventManager::sendEvent(const Event &event) {
	std::stringstream ss;
	ss << "[EventManager] sendEvent: " << EVENT_TO_STRING(event.type); // << " = " << (int) event.type;

	if(event.data != -1)
		ss << ", data: " << std::to_string(event.data);

	if (subscribers.find(event.type) != subscribers.end()) {
		Logger::debug("[EventManager] Notifiying " + std::to_string(subscribers[event.type].size()) + " subscribers about Event " + EVENT_TO_STRING(event.type));
		int i = 1;
		for(const auto& callback : subscribers[event.type]) {
			Logger::debug("[EventManager] Notifiying subscriber #" + std::to_string(i++));
			callback(event);
		}
	} else{
		ss << " -> No subscribers for Event!";
	}
	Logger::debug(ss.str());
}

int EventManager::start() {
	return 0;
}
