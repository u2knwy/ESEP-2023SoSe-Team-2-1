/* 
 * File:   UDPConfigurationFileReader.h
 * @author Lehmann
 * @date July 7, 2019
 */

#ifndef UDPCONFIGURATIONFILEREADER_H
#define UDPCONFIGURATIONFILEREADER_H

#include "UDPConfiguration.h"
#include <string>
#include <vector>
#include <utility>

class UDPConfigFileReader : public UDPConfiguration {
private:
    unsigned int portOffset;
    std::string targetIP;
    uint16_t targetPort;
    std::string localIP;
    uint16_t localPort;
    std::vector<std::pair<std::string, uint16_t>> reportTargets;
    unsigned int report_target_index = 0;
public:
    UDPConfigFileReader();
    UDPConfigFileReader(unsigned int offset);
    UDPConfigFileReader(std::string configfile);
    UDPConfigFileReader(unsigned int offset, std::string configfile);
    virtual ~UDPConfigFileReader() {};
    void showConfig();
    struct in_addr getTargeHostIP() override; // Target address in network byte order
    uint16_t getTargetHostPort() override; // Port in network byte order
    uint16_t getLocalPort()override; // Receiving port in network byte order
    virtual struct in_addr getReportHostIP(); // Target address in network byte order
    virtual uint16_t getReportHostPort(); // Port in network byte order
    virtual unsigned int getNumberReportHosts();  // Number of hosts to report to
    virtual struct in_addr getNextReportHostIP(); // Next Target address in network byte order
    virtual uint16_t getNextReportHostPort();     // Next Port in network byte order
};

#endif /* UDPCONFIGURATIONFILEREADER_H */

