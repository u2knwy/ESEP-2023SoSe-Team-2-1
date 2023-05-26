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

EventManager::EventManager() : server_coid(-1) {
	isMaster = Configuration::getInstance().systemIsMaster();
}

EventManager::~EventManager() {
	// TODO Auto-generated destructor stub
}

void EventManager::subscribe(EventType event, EventCallback callback) {

}

void EventManager::unsubscribe(EventType event, EventCallback callback) {

}

void EventManager::sendEvent(const EventData &event) {
	std::stringstream ss;
	ss << "[EventManager] sendEvent: " << EVENT_TO_STRING(event.event);
	if(!event.msg.empty())
		ss << ", message: " << event.msg;
	if(event.data != -1)
		ss << ", data: " << std::to_string(event.data);
	Logger::info(ss.str());
}

int EventManager::start() {
	return 0;
}
