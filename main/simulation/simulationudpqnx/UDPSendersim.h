/*
 * simdorphandler.h
 *
 *  Created on: 01.05.2020
 *      Author: Lehmann
 */

#ifndef SRC_SIMUDP_UDPSENDERSIM_H_
#define SRC_SIMUDP_UDPSENDERSIM_H_

#include "isimdrophandler.h"
#include "isiminitcompleteobserver.h"
#include "UDPConfiguration.h"
#include <memory>

class UDPSenderSim: public ISimDropHandler, public ISimInitCompleteObserver {
private:
    UDPConfiguration &conf;
    int senderSocket = 0;
    struct sockaddr_in fullTargetAddress;
public:
    UDPSenderSim(UDPConfiguration &conf);
    virtual ~UDPSenderSim();
    int init();
    void dropEnd(shared_ptr<SimItem> dropeditem) override;
    void initCompleted() override;
};

#endif /* SRC_SIMULATIONCORE_SIMDROPHANDLER_H_ */
