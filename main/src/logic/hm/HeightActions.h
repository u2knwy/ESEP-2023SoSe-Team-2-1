/*
 * HeightActions.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#ifndef SRC_LOGIC_HM_HEIGHTACTIONS_H_
#define SRC_LOGIC_HM_HEIGHTACTIONS_H_

#include "events/eventtypes_enum.h"
#include "configuration/Configuration.h"
#include "HeightContextData.h"
#include "events/EventManager.h"

#include <memory>

class HeightActions {
public:
	HeightActions(HeightContextData* data, std::shared_ptr<EventManager> mngr);
	virtual ~HeightActions();
	void setMotorSlow(bool slow);
	void sendHeightResult();
private:
	HeightContextData* data;
	std::shared_ptr<EventManager> eventManager;
	bool isMaster;
};

#endif /* SRC_LOGIC_HM_HEIGHTACTIONS_H_ */
