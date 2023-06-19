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
#include <atomic>
#include <thread>

#include <sys/dispatch.h>

class EventManager {
public:
	using EventCallback = std::function<void(const Event &)>;

	EventManager();
	virtual ~EventManager();

	/**
	 * Connects a client by creating a new connection to the internal Channel
	 *
	 * @return Connection ID - Client can use this to send events to the EventManager
	 */
	int connectInternalClient();

	/**
	 * Subscribe an Event to an EventHandler
	 *
	 * @param type EventType to subscribe to
	 * @param callback Function to call if the event has occurred
	 */
	void subscribe(EventType type, EventCallback callback);

	/**
	 * Subscribe to be notified about all events
	 *
	 * @return number of events subscribed
	 */
	int subscribeToAllEvents(EventCallback callback);

	/**
	 * Unsubscribe from an event
	 *
	 * @param type EventType to unsubscribe from
	 * @param callback Callback function to remove
	 */
	void unsubscribe(EventType type, EventCallback callback);

	/**
	 * Handle a received event
	 *
	 * @param event Event to handle
	 */
	void handleEvent(const Event &event);

	/**
	 * Send an Event to the other system via GNS
	 *
	 * @param event Event to send externally
	 */
	void sendEvent(const Event &event);

	/**
	 * Starts the "Receive internal Events" thread
	 *
	 * @return 0 if start was successful
	 */
	int start();

	/**
	 * Stops the "Receive internal Events" thread.
	 * Blocks until the thread has stopped
	 *
	 * @return 0 if stop was successful
	 */
	int stop();
private:
	bool isMaster;
	int internal_chid;
	int internal_coid;
	int server_coid; // for GNS connection?
	std::atomic<bool> rcvInternalRunning;
	std::thread thRcvInternal;
	std::map<EventType, std::vector<EventCallback>> subscribers;
	std::mutex mtx;
	name_attach_t *attachedServer;
	void openInternalChannel();
	void connectGNS();
	void disconnectGNS();
	void rcvInternalEventsThread();
};
