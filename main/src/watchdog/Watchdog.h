/*
 * Watchdog.h
 *
 *  Created on: 13.06.2023
 *      Author: Maik
 */
#pragma once

#include "events/EventManager.h"
#include "events/IEventHandler.h"
#include <memory>
#include <atomic>
#include <thread>

#define WD_SEND_INTERVAL_SEC 1
#define WD_TIMEOUT_SEC 3

class Watchdog : public IEventHandler {
public:
	Watchdog(std::shared_ptr<EventManager> eventManager);
	virtual ~Watchdog();
	void handleEvent(Event event) override;
	void sendingThread();
	void receivingThread();
	void start();
	void stop();
private:
	std::shared_ptr<EventManager> eventManager;
	bool isMaster;
	bool connectionActive;
	std::thread th_receive;
	std::thread th_send;
	std::atomic<bool> sendingRunning;
	std::atomic<bool> receivingRunning;
	std::chrono::steady_clock::time_point lastReceiveTime;
	void heartbeatReceived();
	void sendHeartbeat();
};
