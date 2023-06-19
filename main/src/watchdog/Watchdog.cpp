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

    if (connect(eventManager)) {
        Logger::debug("[Watchdog] Connected to EventManager");
    } else {
        Logger::error("[Watchdog] Error while connecting to EventManager");
    }

    if (isMaster) {
        eventManager->subscribe(
            EventType::WD_S_HEARTBEAT,
            std::bind(&Watchdog::handleEvent, this, std::placeholders::_1));
    } else {
        eventManager->subscribe(
            EventType::WD_M_HEARTBEAT,
            std::bind(&Watchdog::handleEvent, this, std::placeholders::_1));
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
    stop();
    th_send = std::thread(&Watchdog::sendingThread, this);
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
    Logger::debug("[WD] Started sending heartbeats...");
    sendingRunning = true;
    while (sendingRunning) {
        Logger::debug("[WD] Sending heartbeat...");
        sendHeartbeat();
        std::this_thread::sleep_for(std::chrono::seconds(WD_SEND_INTERVAL_SEC));
    }
    sendingRunning = false;
    Logger::debug("[WD] Stopped sending heartbeats");
}

void Watchdog::sendHeartbeat() {
    if (isMaster) {
        sendEvent(Event{WD_M_HEARTBEAT});
    } else {
        sendEvent(Event{WD_S_HEARTBEAT});
    }
}

void Watchdog::receivingThread() {
    using namespace std::chrono;
    Logger::debug("[WD] Started receiving heartbeats...");
    receivingRunning = true;
    lastReceiveTime = steady_clock::now();
    while (receivingRunning) {
        // every second: check if timeout has occurred
        std::this_thread::sleep_for(seconds(1));
        const auto now = steady_clock::now();
        int elapsed_sec = duration_cast<seconds>(now - lastReceiveTime).count();
        if (elapsed_sec > WD_TIMEOUT_SEC) {
            Logger::debug("[WD] Heartbeat received " +
                          std::to_string(elapsed_sec) +
                          " seconds ago -> ERROR!");
            if (!connectionLost) {
                Logger::error("[WD] Connection lost! (Timeout)");
                connectionLost = true;
                isMaster ? sendEvent(WD_M_CONN_LOST)
                         : sendEvent(WD_S_CONN_LOST);
            }
        } else {
            Logger::debug("[WD] Heartbeat received " +
                          std::to_string(elapsed_sec) + " seconds ago -> OK!");
            if (connectionLost) {
                Logger::info("[WD] Connection reestablished");
                connectionLost = false;
                isMaster ? sendEvent(WD_M_CONN_REESTABLISHED)
                         : sendEvent(WD_S_CONN_REESTABLISHED);
            }
        }
    }
    Logger::debug("[WD] Stopped receiving heartbeats");
    receivingRunning = false;
}

void Watchdog::heartbeatReceived() {
    using namespace std::chrono;
    Logger::debug("[WD] Received heartbeat");
    lastReceiveTime = steady_clock::now();
}
