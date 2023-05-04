/*
 * UDPReceiverThreadSimItemHandling.cpp
 *
 *  Created on: 29.04.2020
 *      Author: Lehmann
 */

#include "UDPReceiverThreadSimItemHandling.h"
#include "simconfquery.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

UDPReceiverThreadSimItemHandling::UDPReceiverThreadSimItemHandling(UDPConfiguration &conf, SimJSONMessageHandler *msghandler) :
        conf(conf), receiverSocket(0), run(true), initialMessageReceived(false), msghandler(msghandler) {
    // nothing to be done
}

UDPReceiverThreadSimItemHandling::~UDPReceiverThreadSimItemHandling() {
    if (receiverSocket != 0) {
        close(receiverSocket);
    }
}

void UDPReceiverThreadSimItemHandling::operator()() {
    receive();
}

void UDPReceiverThreadSimItemHandling::receive() {
    receiverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in local_address;
    memset(&local_address, 0, sizeof(local_address));
    unsigned int local_address_len = sizeof(struct sockaddr_in);
    local_address.sin_family = AF_INET;
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);
    local_address.sin_port = conf.getLocalPort();

    //bind socket to port
    int rc = ::bind(receiverSocket, (struct sockaddr*) &local_address,
            local_address_len);
    if (rc < 0) {
        cout << "<SIM> Error, socket binding error: " << errno << endl;
    } else {
        char buffer[1500];
        uint16_t value = 0;
        SimItemHandlingAction action;

        while (run) {
            buffer[0]=0;
            int recv_len = recvfrom(receiverSocket, buffer, sizeof(buffer), 0,
                    (struct sockaddr *) &local_address, &local_address_len);
            buffer[recv_len] = 0; // force zero-termination.

            value = 0;
            if(SIMCONFQUERRY_ISACTIVE(showactions)){
            	cout << "<Sim> recv action:" << buffer << endl;
            }
            if(!initialMessageReceived){
                initialMessageReceived = true;

            }
            // handover to message handling
            if(msghandler!=nullptr){
                msghandler->dispatchMessage(std::string(buffer));
            }

            if (value == 0xFFFF) {
                run = false;
            }
        }
    }
    close(receiverSocket);
    receiverSocket = 0;
}



