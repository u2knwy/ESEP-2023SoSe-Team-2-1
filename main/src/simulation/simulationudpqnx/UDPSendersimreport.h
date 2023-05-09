/*
 * simdorphandler.h
 *
 *  Created on: 01.05.2020
 *      Author: Lehmann
 */

#ifndef SRC_SIMUDP_UDPSENDERSIMREPORT_H_
#define SRC_SIMUDP_UDPSENDERSIMREPORT_H_

#include "isimulationreporthandler.h"
#include "UDPConfigFileReader.h"

class UDPSenderSimReport: public ISimulationReportHandler {
private:
    UDPConfigFileReader &conf;
    int senderSocket = 0;
    struct sockaddr_in fullTargetAddress;
public:
    UDPSenderSimReport(UDPConfigFileReader &conf);
    virtual ~UDPSenderSimReport();
    int init();
    void handlereport(const std::string jsonreport) override;
};

#endif
