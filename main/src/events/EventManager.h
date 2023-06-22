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
#include <sys/neutrino.h>
#include <sys/iofunc.h>

#define ATTACH_POINT_LOCAL_M "EventMgrMaster"
#define ATTACH_POINT_LOCAL_S "EventMgrSlave"

// qnx message declarations
typedef struct _pulse header_t;
/* Second header: used by application - if required */
typedef struct
{
    int size;  // size of data block
    int data;
    int eventnr;// some counter used by the application
               // further data fields required by our application
} app_header_t;


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
	void sendExternalEvent(const Event &event);

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

	void connectToService(const std::string& name);
private:
	bool isMaster;
	int internal_chid;
	int internal_coid;
	int server_coid; // for GNS connection?
	bool externConnected;
	std::atomic<bool> rcvInternalRunning;
	std::thread thRcvInternal;
    std::atomic<bool> rcvExternalRunning;
    std::thread thRcvExternal;
	std::map<EventType, std::vector<EventCallback>> subscribers;
	std::mutex mtx;
	name_attach_t *attachedService;
	std::string ownServiceName;
	std::string otherServiceName;
	void openInternalChannel();
	void sendToSelf(Event event);
	void createService();
	void stopService();
	void disconnectFromService();
	void rcvInternalEventsThread();
    void rcvExternalEventsThread();
    void handle_pulse(header_t hdr, int rcvid);
    void handle_ONX_IO_msg(header_t hdr, int rcvid);
    void handle_app_msg(header_t hdr, int rcvid);
};
