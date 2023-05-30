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


class EventManager {
public:
	using EventCallback = std::function<void(const Event &)>;
	//typedef void (*EventCallback)(Event);
	EventManager();
	virtual ~EventManager();
	void subscribe(EventType type, EventCallback callback);
	void unsubscribe(EventType type, EventCallback callback);
	void sendEvent(const Event &event);
	int start();
private:
	bool isMaster;
	int server_coid;
	std::map<EventType, std::vector<EventCallback>> subscribers;
	std::mutex mtx;
};
