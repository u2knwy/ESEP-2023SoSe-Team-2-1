/*
 * EventManagerMock.h
 *
 *  Created on: 22.06.2023
 *      Author: Maik
 */
#pragma once

#include "events/IEventManager.h"

class EventManagerMock : public IEventManager {
public:
	EventManagerMock();
	~EventManagerMock() override;
	int connectInternalClient() override;
	void subscribe(EventType type, EventCallback callback) override;
	int subscribeToAllEvents(EventCallback callback) override;
	void unsubscribe(EventType type, EventCallback callback) override;
	void handleEvent(const Event &event) override;
	void sendExternalEvent(const Event &event) override;
	int start() override;
	int stop() override;
	void connectToService(const std::string& name) override;

	Event getLastHandledEvent();
	bool lastHandledEventsContain(const Event& event);
	void clearLastHandledEvents();
private:
	std::vector<Event> lastEvents;
};
