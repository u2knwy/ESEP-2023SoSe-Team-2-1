/*
 * EventManagerMock.cpp
 *
 *  Created on: 22.06.2023
 *      Author: Maik
 */

#include "EventManagerMock.h"
#include <iostream>
#include <string>
#include "logger/logger.hpp"

using namespace std;

static void log(const std::string& msg) {
	cout << "[EventManagerMock] " << msg << endl;
}

EventManagerMock::EventManagerMock() {
	log("constructed");
}

EventManagerMock::~EventManagerMock() {
	log("destructed");
}

int EventManagerMock::connectInternalClient() {
	log("connectInternalClient");
	return 0;
}

void EventManagerMock::subscribe(EventType type, EventCallback callback) {
    if (subscribers.find(type) == subscribers.end()) {
        // If event doesn't exist yet, create a new list and add it to the map
        subscribers[type] = std::vector<EventCallback>();
    }
    subscribers[type].push_back(callback);
}

int EventManagerMock::subscribeToAllEvents(EventCallback callback) {
    int nEvents = 0;
    for (int i = static_cast<int>(EventType::PULSE_STOP_THREAD);
         i <= static_cast<int>(EventType::ERROR_S_SELF_SOLVED); i++) {
        EventType eventType = static_cast<EventType>(i);
        subscribe(eventType, callback);
        nEvents++;
    }
    return nEvents;
}

void EventManagerMock::unsubscribe(EventType type, EventCallback callback) {

}

void EventManagerMock::handleEvent(const Event &event) {
	lastEvents.push_back(event);

    std::stringstream ss;
    ss << "[EventManagerMock] handleEvent: " << EVENT_TO_STRING(event.type);
    if (event.data != -1)
        ss << ", data: " << event.data;

    if (subscribers.find(event.type) != subscribers.end()) {
        int i = 1;
        for (const auto &callback : subscribers[event.type]) {
            callback(event);
        }
    } else {
        ss << " -> No subscribers for Event!";
    }
    Logger::debug(ss.str());
}

void EventManagerMock::sendExternalEvent(const Event &event) {
	log("sendExternalEvent");
}

int EventManagerMock::start() {
	log("start");
	return 0;
}

int EventManagerMock::stop() {
	log("stop");
	return 0;
}

void EventManagerMock::connectToService(const std::string& name) {
	log("connectToService");
}

Event EventManagerMock::getLastHandledEvent() {
	return lastEvents.back();
}

bool EventManagerMock::lastHandledEventsContain(const Event& event) {
	for(auto const& ev: lastEvents) {
		if(ev.type == event.type && ev.data == event.data && ev.additional_data == event.additional_data) {
			return true;
		}
	}
	return false;
}

void EventManagerMock::clearLastHandledEvents() {
	std::vector<Event>().swap(lastEvents);
}
