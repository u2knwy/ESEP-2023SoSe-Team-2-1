/*
 * Watchdog.cpp
 *
 *  Created on: 13.06.2023
 *      Author: Maik
 */

#include "Watchdog.h"
#include "configuration/Configuration.h"
#include "logger/logger.hpp"

#include <chrono>
#include <sys/neutrino.h>
#include <thread>


Watchdog::Watchdog(std::shared_ptr<EventManager> eventManager) {
    this->eventManager = eventManager;
    this->isMaster = Configuration::getInstance().systemIsMaster();

    if (!connect(eventManager)) {
        throw std::runtime_error("[Watchdog] Error while connecting to EventManager");
    }

    if (isMaster) {
        Logger::debug("Watchdog Subscribe to Slave Heartbeat");
        eventManager->subscribe(EventType::WD_S_HEARTBEAT, std::bind(&Watchdog::handleEvent, this, std::placeholders::_1));
    } else {
        Logger::debug("Watchdog Subscribe to Master Heartbeat");
        eventManager->subscribe(EventType::WD_M_HEARTBEAT, std::bind(&Watchdog::handleEvent, this, std::placeholders::_1));
    }
}

Watchdog::~Watchdog() {
    stop();
    disconnect();
}

void Watchdog::handleEvent(Event event) {
    switch (event.type) {
    case WD_M_HEARTBEAT:
    case WD_S_HEARTBEAT:
        heartbeatReceived();
        break;
    default:
        break;
    }
}

void Watchdog::start() {
   // th_send = std::thread(&Watchdog::sendingThread, this);
    th_receive = std::thread(&Watchdog::receivingThread, this);
}

void Watchdog::stop() {
    receivingRunning = false;
    if (th_receive.joinable()) {
        th_receive.join();
    }
    sendingRunning = false;
    if (th_send.joinable()) {
        th_send.join();
    }
}

void Watchdog::sendingThread() {
	std::this_thread::sleep_for(std::chrono::seconds(5));
    Logger::debug("[WD] Started sending heartbeats...");
    sendingRunning = true;
    while (sendingRunning) {
        sendHeartbeat();
        std::this_thread::sleep_for(std::chrono::milliseconds(WD_SEND_INTERVAL_MILLIS));
    }
    sendingRunning = false;
    Logger::debug("[WD] Stopped sending heartbeats");
}

void Watchdog::sendHeartbeat() {
    if (isMaster) {
        Logger::debug("sending heartbeat to slave");
        sendEvent(Event{WD_M_HEARTBEAT});
    } else if(!isMaster) {
        Logger::debug("sending heartbeat to master");
        sendEvent(Event{WD_S_HEARTBEAT});
    }
}

void Watchdog::receivingThread() {
    using namespace std::chrono;
	std::this_thread::sleep_for(std::chrono::seconds(5));
    Logger::debug("[WD] Started receiving heartbeats...");
    receivingRunning = true;
    while (receivingRunning) {
        // every second: check if timeout has occurred
        std::this_thread::sleep_for(milliseconds(WD_TIMEOUT_MILLIS));
        if (heartBeatreceived) {
        heartBeatreceived = false;
        sendHeartbeat();
        }
        else {
        		Logger::error("[WD] Connection lost! (Timeout)");
        		receivingRunning = false;
        		sendingRunning = false;
        		heartBeatreceived = false;
        		sendEvent(WD_CONN_LOST);

        		if(isMaster){sendEvent(Event{EventType::ERROR_M_SELF_SOLVABLE});}
        		else{sendEvent(Event{EventType::ERROR_S_SELF_SOLVABLE});}
        		Logger::error("trying to reconnect...");

            }

    }
    Logger::debug("[WD] Stopped receiving heartbeats");
    receivingRunning = false;
}

void Watchdog::heartbeatReceived() {

    heartBeatreceived = true;

}
