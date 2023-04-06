/*
 * UDPSimreporthandler.cpp
 *
 *  Created on: 29.05.2020
 *      Author: Lehmann
 */

#include <iostream>

#include "UDPSendersimreport.h"

using namespace std;

UDPSenderSimReport::UDPSenderSimReport(UDPConfigFileReader &simudpconf) :
        conf(simudpconf) {
    memset(&fullTargetAddress, 0, sizeof(fullTargetAddress));
    init();
}

UDPSenderSimReport::~UDPSenderSimReport() {
    close(senderSocket);
}
int UDPSenderSimReport::init() {
    // Create sender socket
    senderSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (senderSocket < 0) {
        cout << "<SIM> Error, no socket available" << endl;
    }
    //std::cout << "Socket:" << senderSocket << std::endl;
    fullTargetAddress.sin_family = AF_INET;
    fullTargetAddress.sin_addr = conf.getReportHostIP();
    fullTargetAddress.sin_port = conf.getReportHostPort();
    return senderSocket;
}

void UDPSenderSimReport::handlereport(std::string report) {
    if (senderSocket >= 0) {
        int target_address_len = sizeof (struct sockaddr_in);
        
        // Send to first target
        fullTargetAddress.sin_addr = conf.getReportHostIP();
        fullTargetAddress.sin_port = conf.getReportHostPort();

        sendto(senderSocket, report.c_str(), report.size(), 0,
                (struct sockaddr *) &fullTargetAddress, target_address_len);
        
        // Send to additional targets
        for (unsigned int i = 1; i < conf.getNumberReportHosts(); i++) {
            fullTargetAddress.sin_addr = conf.getNextReportHostIP();
            fullTargetAddress.sin_port = conf.getNextReportHostPort();

            sendto(senderSocket, report.c_str(), report.size(), 0,
                    (struct sockaddr *) &fullTargetAddress, target_address_len);

        }
    }
}
