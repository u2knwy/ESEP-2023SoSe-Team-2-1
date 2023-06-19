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
#include "events/EventManager.h"
#include "events/EventSender.h"
#include "events/eventtypes_enum.h"


#include <memory>

class HeightActions : public EventSender {
  public:
    HeightActions(HeightContextData *data, std::shared_ptr<EventManager> mngr);
    virtual ~HeightActions();
    void sendMotorSlowRequest(bool slow);
    void sendHeightResult();

  private:
    HeightContextData *data;
    std::shared_ptr<EventManager> eventManager;
    bool isMaster;
};

#endif /* SRC_LOGIC_HM_HEIGHTACTIONS_H_ */
