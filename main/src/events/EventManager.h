/*
 * EventManager.h
 *
 *  Created on: 25.05.2023
 *      Author: Maik
 */
#pragma once

#include "events.h"
#include "eventtypes_enum.h"
#include <functional>
#include <string>

typedef struct EventData {
    EventType event;
    int data;
    std::string msg;
};

class EventManager {
public:
	using EventCallback = std::function<void(const EventData &)>;
	EventManager();
	virtual ~EventManager();
	void subscribe(EventType event, EventCallback callback);
	void unsubscribe(EventType event, EventCallback callback);
	void sendEvent(const EventData &event);
	int start();
private:
	bool isMaster;
	int server_coid;
};
