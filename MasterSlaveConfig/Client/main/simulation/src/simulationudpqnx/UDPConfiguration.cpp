/*
 * Configuration.cpp
 *
 *  Created on: 25.06.2019
 *      Author: Lehmann
 */

#include "UDPConfiguration.h"
extern "C" {
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
}
#define HOST_IMAGE_RECEIVERIP "192.168.101.11"
#define HOST_IMAGE_RECEIVERPORT 50000
#define SELF_IMAGE_RECEIVERPORT 40000

UDPConfigurationStatic::UDPConfigurationStatic() : portOffset(0) {
    // nothing to be done
}

UDPConfigurationStatic::UDPConfigurationStatic(unsigned int offset) : portOffset(offset) {
    // nothing to be done
}

struct in_addr UDPConfigurationStatic::getTargeHostIP(){
    struct in_addr addr;
    inet_aton(HOST_IMAGE_RECEIVERIP, &addr);
    return addr;
}

uint16_t UDPConfigurationStatic::getTargetHostPort(){
     return htons(HOST_IMAGE_RECEIVERPORT+portOffset);
}
uint16_t UDPConfigurationStatic::getLocalPort(){
     return htons(SELF_IMAGE_RECEIVERPORT+portOffset);
}

