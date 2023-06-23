/*
 * HeightActions.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#ifndef SRC_LOGIC_HM_HEIGHTACTIONS_H_
#define SRC_LOGIC_HM_HEIGHTACTIONS_H_

#include "HeightContextData.h"
#include "configuration/Configuration.h"
#include "events/IEventManager.h"
#include "events/IEventSender.h"
#include "events/eventtypes_enum.h"

#include <memory>

class HeightActions {
  public:
    HeightActions(HeightContextData *data, IEventSender* sender, std::shared_ptr<IEventManager> mngr);
    virtual ~HeightActions();
    void sendMotorSlowRequest(bool slow);
    void sendHeightResult();

    std::shared_ptr<IEventManager> eventManager;
  private:
    IEventSender* sender;
    HeightContextData *data;
    bool isMaster;
};

#endif /* SRC_LOGIC_HM_HEIGHTACTIONS_H_ */
