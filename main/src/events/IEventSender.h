/*
 * EventSender.h
 *
 *  Created on: 18.06.2023
 *      Author: Maik
 */
#pragma once

#include "events/IEventManager.h"
#include "events/events.h"
#include "logger/logger.hpp"

class IEventSender {
  public:
	IEventSender() { coid = -1; }

    virtual ~IEventSender() {
    	std::cout << "IEventSender Destructor called" << std::endl;
    };

    /**
     * Connects to the EventManager by attaching to its internal channel.
     * We will send future events to the EventManager by our own connection ID.
     *
     * @param evm Reference to the EventManager instance
     * @return true if connecting was successful.
     */
    virtual bool connect(std::shared_ptr<IEventManager> evm) = 0;

    /**
     * Disconnects from the EventManager channel if connected
     */
    virtual void disconnect() = 0;

    /**
     * Sends an event to the EventManager
     *
     * @param event Event to send
     * @return true if send was successful
     */
    virtual bool sendEvent(Event event) = 0;
  private:
    int coid;
};
