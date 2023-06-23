/*
 * IEventManager.h
 *
 *  Created on: 22.06.2023
 *      Author: Maik
 */
#pragma once

#include "events.h"
#include <functional>
#include <string>
#include <map>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <utility>
#include <vector>

class IEventManager {
public:
	using EventCallback = std::function<void(const Event &)>;

	virtual ~IEventManager() = default;

	/**
	 * Connects a client by creating a new connection to the internal Channel
	 *
	 * @return Connection ID - Client can use this to send events to the EventManager
	 */
	virtual int connectInternalClient() = 0;

	/**
	 * Subscribe an Event to an EventHandler
	 *
	 * @param type EventType to subscribe to
	 * @param callback Function to call if the event has occurred
	 */
	virtual void subscribe(EventType type, EventCallback callback) = 0;

	/**
	 * Subscribe to be notified about all events
	 *
	 * @return number of events subscribed
	 */
	virtual int subscribeToAllEvents(EventCallback callback) = 0;

	/**
	 * Unsubscribe from an event
	 *
	 * @param type EventType to unsubscribe from
	 * @param callback Callback function to remove
	 */
	virtual void unsubscribe(EventType type, EventCallback callback) = 0;

	/**
	 * Handle a received event
	 *
	 * @param event Event to handle
	 */
	virtual void handleEvent(const Event &event) = 0;

	/**
	 * Send an Event to the other system via GNS
	 *
	 * @param event Event to send externally
	 */
	virtual void sendExternalEvent(const Event &event) = 0;

	/**
	 * Starts the "Receive internal Events" thread
	 *
	 * @return 0 if start was successful
	 */
	virtual int start() = 0;

	/**
	 * Stops the "Receive internal Events" thread.
	 * Blocks until the thread has stopped
	 *
	 * @return 0 if stop was successful
	 */
	virtual int stop() = 0;

	virtual void connectToService(const std::string& name) = 0;

protected:
	std::map<EventType, std::vector<EventCallback>> subscribers;

private:
	bool isMaster;
	bool externConnected;
	int internal_chid;
	int internal_coid;
	int server_coid; // for GNS connection?
};
