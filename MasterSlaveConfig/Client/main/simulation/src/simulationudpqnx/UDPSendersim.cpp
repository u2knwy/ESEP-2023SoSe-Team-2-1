/*
 * simdrophandler.cpp
 *
 *  Created on: 01.05.2020
 *      Author: Lehmann
 */

#include "simitemhandlingaction.h"
#include "simctrlaction.h"
#include "simconfquery.h"
#include "UDPSendersim.h"
#include <iostream>

using namespace std;

UDPSenderSim::UDPSenderSim(UDPConfiguration &simudpconf) :
        conf(simudpconf) {
    memset(&fullTargetAddress, 0, sizeof(fullTargetAddress));
    init();
}

UDPSenderSim::~UDPSenderSim() {
    close(senderSocket);
}
int UDPSenderSim::init() {
    // Create sender socket
    senderSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (senderSocket < 0) {
        cout << "<SIM> Error, no socket available" << endl;
    }
    //std::cout << "Socket:" << senderSocket << std::endl;
    fullTargetAddress.sin_family = AF_INET;
    fullTargetAddress.sin_addr = conf.getTargeHostIP();
    fullTargetAddress.sin_port = conf.getTargetHostPort();
    return senderSocket;
}

void UDPSenderSim::dropEnd(shared_ptr<SimItem> dropeditem) {
    if (senderSocket >= 0) {
    	dropeditem->evalFlip();
        SimItemHandlingAction action(0, dropeditem->kind);
        action.x = 0.0;
        action.y = dropeditem->y;
        string message = action.toJSONString();
        if(SIMCONFQUERRY_ISACTIVE(showactions)){
        	std::cout << "<Sim> send action:" << message << std::endl;
        }
        int target_address_len = sizeof(struct sockaddr_in);

        sendto(senderSocket, message.c_str(), message.size(), 0,
                (struct sockaddr *) &fullTargetAddress, target_address_len);
    }
}
void UDPSenderSim::initCompleted(){
    if (senderSocket >= 0) {
        SimCtrlAction action("start");
        string message = action.toJSONString();
        if(SIMCONFQUERRY_ISACTIVE(showactions)){
        	std::cout << "<Sim> send action:" << message << std::endl;
        }
        int target_address_len = sizeof(struct sockaddr_in);

        sendto(senderSocket, message.c_str(), message.size(), 0,
                (struct sockaddr *) &fullTargetAddress, target_address_len);
    }
}
