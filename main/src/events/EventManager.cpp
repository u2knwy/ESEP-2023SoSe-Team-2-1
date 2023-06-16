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
}

EventManager::~EventManager() {
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
