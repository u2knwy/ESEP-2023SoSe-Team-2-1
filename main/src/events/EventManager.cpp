/*
 * EventManager.cpp
 *
 *  Created on: 25.05.2023
 *      Author: Maik
 */

#include "EventManager.h"
#include "configuration/Configuration.h"

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

}

int EventManager::start() {
	return 0;
}
