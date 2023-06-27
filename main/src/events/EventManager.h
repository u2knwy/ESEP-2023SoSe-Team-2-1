/*
 * EventManager.h
 *
 *  Created on: 25.05.2023
 *      Author: Maik
 */
#pragma once

#include "IEventManager.h"

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


class EventManager : public IEventManager {
public:
	EventManager();
	~EventManager() override;

	/**
	 * Connects a client by creating a new connection to the internal Channel
	 *
	 * @return Connection ID - Client can use this to send events to the EventManager
	 */
	int connectInternalClient() override;

	void subscribe(EventType type, EventCallback callback) override;

	int subscribeToAllEvents(EventCallback callback) override;

	void unsubscribe(EventType type, EventCallback callback) override;

	/**
	 * Handle a received event
	 *
	 * @param event Event to handle
	 */
	void handleEvent(const Event &event) override;

	/**
	 * Send an Event to the other system via GNS
	 *
	 * @param event Event to send externally
	 */
	void sendExternalEvent(const Event &event) override;

	/**
	 * Starts the "Receive internal Events" thread
	 *
	 * @return 0 if start was successful
	 */
	int start() override;

	/**
	 * Stops the "Receive internal Events" thread.
	 * Blocks until the thread has stopped
	 *
	 * @return 0 if stop was successful
	 */
	int stop() override;

	void connectToService(const std::string& name) override;
	void connectionLost();
	void tryreconnect();
private:
	bool isMaster;
	int internal_chid;
	int internal_coid;
	int server_coid; // for GNS connection?
	bool externConnected;
	bool disconnected = false;
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
