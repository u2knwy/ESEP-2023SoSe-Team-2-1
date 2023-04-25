/*
 * UDPReceiverThreadSimItemHandling.h
 *
 *  Created on: 29.04.2020
 *      Author: Lehmann
 */

#ifndef SRC_UDP_UDPRECEIVERTHREADSIMITEMHANDLING_H_
#define SRC_UDP_UDPRECEIVERTHREADSIMITEMHANDLING_H_


#include "UDPConfiguration.h"
#include "../simulationcore/simjsonmessagehandler.h"

class UDPReceiverThreadSimItemHandling {
private:
    UDPConfiguration &conf;
private:
    int receiverSocket;
    bool run;
    bool initialMessageReceived;
    SimJSONMessageHandler *msghandler;
public:
    UDPReceiverThreadSimItemHandling(UDPConfiguration &conf, SimJSONMessageHandler *msghandler=nullptr);
    virtual ~UDPReceiverThreadSimItemHandling();

    void operator()();
private:
    void receive();
};



#endif /* SRC_UDP_UDPRECEIVERTHREADSIMITEMHANDLING_H_ */
