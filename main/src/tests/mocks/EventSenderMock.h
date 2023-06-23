/*
 * EventSenderMock.h
 *
 *  Created on: 22.06.2023
 *      Author: Maik
 */

#ifndef SRC_TESTS_EVENTSENDERMOCK_H_
#define SRC_TESTS_EVENTSENDERMOCK_H_

#include "events/IEventSender.h"

class EventSenderMock : public IEventSender {
public:
	EventSenderMock();
	~EventSenderMock() override;
	bool connect(std::shared_ptr<IEventManager> evm) override;
	void disconnect() override;
	bool sendEvent(Event event) override;
private:
	std::shared_ptr<IEventManager> evm;
};

#endif /* SRC_TESTS_EVENTSENDERMOCK_H_ */
