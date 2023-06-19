/*
 * EventManager.cpp
 *
 *  Created on: 25.05.2023
 *      Author: Maik
 */

#include "EventManager.h"
#include "configuration/Configuration.h"
#include "logger/logger.hpp"
#include "events.h"

#include <string>
#include <iostream>
#include <sstream>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <sys/iofunc.h>

#define ATTACH_POINT_LOCAL_M "EventMgrMaster"
#define ATTACH_POINT_LOCAL_S "EventMgrSlave"

#define ATTACH_POINT_GNS_M "/dev/name/global/EventMgrMaster"
#define ATTACH_POINT_GNS_S "/dev/name/global/EventMgrSlave"


EventManager::EventManager() : internal_chid(-1), internal_coid(-1), server_coid(-1) {
	isMaster = Configuration::getInstance().systemIsMaster();
	//connectGNS();
	rcvInternalRunning = false;
	openInternalChannel();
}

EventManager::~EventManager() {
	//disconnectGNS();
	ConnectDetach(internal_coid);
	ChannelDestroy(internal_chid);
}

void EventManager::openInternalChannel() {
	internal_chid = ChannelCreate(0);
	if(internal_chid == -1) {
		Logger::error("[EventManager] Creating internal channel failed");
		throw std::runtime_error("ChannelCreate failed");
	}
	internal_coid = ConnectAttach(0, 0, internal_chid, _NTO_SIDE_CHANNEL, 0);
	if(internal_coid == -1) {
		Logger::error("[EventManager] Attaching to internal channel failed");
		throw std::runtime_error("ConnectAttach failed");
	}
}

int EventManager::connectInternalClient() {
	if(internal_chid == -1) {
		openInternalChannel();
	}
	int coid = ConnectAttach(0, 0, internal_chid, _NTO_SIDE_CHANNEL, 0);
	if(coid == -1) {
		Logger::error("ConnectAttach failed");
	}
	return coid;
}

void EventManager::rcvInternalEventsThread() {
	rcvInternalRunning = true;
	Logger::debug("[EventManager] Ready to receive internal events");
	while(rcvInternalRunning) {
	    _pulse pulse;
		int rtrn = MsgReceivePulse(internal_chid, &pulse, sizeof(pulse), NULL);
		if(rtrn < 0) {
			Logger::error("Error during MsgReceivePulse: " + std::to_string(errno));
		}
		if(pulse.code == PULSE_STOP_THREAD) {
			rcvInternalRunning = false;
			continue;
		}
		Event ev{(EventType) pulse.code, pulse.value.sival_int};
		handleEvent(ev);
	}
	Logger::debug("[EventManager] Stopped receiving internal events");
}

void EventManager::connectGNS() {
	if(isMaster) {
		// create service
		if( (attachedServer = name_attach(NULL, "SERVICE", NAME_FLAG_ATTACH_GLOBAL)) == NULL) {
			Logger::error("name_attach failed");
		}
		Logger::info("Master created GNS Service");
	} else {
		// connect to master service
		if( (server_coid = name_open("SERVICE", NAME_FLAG_ATTACH_GLOBAL)) == -1 ) {
			throw runtime_error("failed to connect to service");
		}
		Logger::info("Slave attached to GNS Service @Master");
	}
}

void EventManager::disconnectGNS() {
	if(isMaster) {
		// terminate service
		if( name_detach(attachedServer, 0) == -1 ) {
			Logger::error("failed detaching server");
		}
	} else {
		// disconnect from master
		if( name_close(server_coid) == -1 ) {
			Logger::error("failed detaching client");
		}
	}
}

void EventManager::subscribe(EventType type, EventCallback callback) {
	if (subscribers.find(type) == subscribers.end()) {
		// If event doesn't exist yet, create a new list and add it to the map
		subscribers[type] = std::vector<EventCallback>();
	}
	subscribers[type].push_back(callback);
}

int EventManager::subscribeToAllEvents(EventCallback callback) {
	int nEvents = 0;
	for (int i = static_cast<int>(EventType::PULSE_STOP_THREAD); i <= static_cast<int>(EventType::ERROR_S_SELF_SOLVED); i++) {
		EventType eventType = static_cast<EventType>(i);
		subscribe(eventType, callback);
		nEvents++;
	}
	return nEvents;
}

void EventManager::unsubscribe(EventType type, EventCallback callback) {

}

void EventManager::handleEvent(const Event &event) {
	std::stringstream ss;
	ss << "[EventManager] handleEvent: " << EVENT_TO_STRING(event.type);

	if(event.data != -1)
		ss << ", data: " << event.data;

	if (subscribers.find(event.type) != subscribers.end()) {
		Logger::debug("[EventManager] Notifiying " + std::to_string(subscribers[event.type].size()) + " subscribers about Event " + EVENT_TO_STRING(event.type));
		int i = 1;
		for(const auto& callback : subscribers[event.type]) {
			Logger::debug("[EventManager] Notifiying subscriber #" + std::to_string(i++));
			callback(event);
		}
	} else{
		ss << " -> No subscribers for Event!";
	}
	Logger::debug(ss.str());
}

void EventManager::sendEvent(const Event &event) {
	// TODO: Send event to other system
}

int EventManager::start() {
	thRcvInternal = std::thread(&EventManager::rcvInternalEventsThread, this);
	return 0;
}

int EventManager::stop() {
	int res = MsgSendPulse(internal_coid, -1, PULSE_STOP_THREAD, 0);
	if(res < 0) {
		Logger::error("[EventManager] Error during MsgSendPulse: " + std::to_string(errno));
	}
	thRcvInternal.join();
	return 0;
}
