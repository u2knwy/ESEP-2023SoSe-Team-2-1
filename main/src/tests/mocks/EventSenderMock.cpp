/*
 * EventSenderMock.cpp
 *
 *  Created on: 22.06.2023
 *      Author: Maik
 */

#include "EventSenderMock.h"

EventSenderMock::EventSenderMock() {
}

EventSenderMock::~EventSenderMock() {}

/**
 * Connects to the EventManager by attaching to its internal channel.
 * We will send future events to the EventManager by our own connection ID.
 *
 * @param evm Reference to the EventManager instance
 * @return true if connecting was successful.
 */
bool EventSenderMock::connect(std::shared_ptr<IEventManager> evm) {
	this->evm = evm;
	return true;
}

/**
 * Disconnects from the EventManager channel if connected
 */
void EventSenderMock::disconnect() {
	this->evm.reset();
}

/**
 * Sends an event to the EventManager
 *
 * @param event Event to send
 * @return true if send was successful
 */
bool EventSenderMock::sendEvent(Event event) {
	evm->handleEvent(event);
	return true;
}
