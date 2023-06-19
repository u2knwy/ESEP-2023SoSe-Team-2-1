/*
 * EventSender.h
 *
 *  Created on: 18.06.2023
 *      Author: Maik
 */
#pragma once

#include <sys/neutrino.h>
#include "logger/logger.hpp"
#include "events/EventManager.h"

class EventSender {
public:
	EventSender() {
		coid = -1;
	}

	virtual ~EventSender() {
	}

	/**
	 * Connects to the EventManager by attaching to its internal channel.
	 * We will send future events to the EventManager by our own connection ID.
	 *
	 * @param Reference to the EventManager instance
	 * @return true if connecting was successful.
	 */
	bool connect(std::shared_ptr<EventManager> evm) {
		coid = evm->connectInternalClient();
		return coid != -1;
	}

	/**
	 * Disconnects from the EventManager channel.
	 *
	 * @return true if disconnect was successful.
	 */
	void disconnect() {
		int ret = ConnectDetach(this->coid);
		if(ret < 0) {
			Logger::error("[EventSender] ConnectDetach failed");
		}
	}

	/**
	 * Sends an event to the EventManager
	 *
	 * @param event Event to send
	 * @return true if send was successful
	 */
	bool sendEvent(Event event) {
		if(coid == -1) {
			Logger::error("It was tried to send an internal event without being connected to the EventManager");
			return false;
		}

		int res = MsgSendPulse(this->coid, -1, (int) event.type, event.data);
		if (res < 0) {
			Logger::error("Failed to send pulse message to EventManager. errno = " + std::to_string(errno));
			return false;
		}
		return true;
	}

private:
	int coid;
};
