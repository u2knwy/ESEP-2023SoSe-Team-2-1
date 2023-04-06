/* 
 * File:   UDPConfigFileReader.cpp
 * @author Lehmann
 * @date July 7, 2019
 */
#include "UDPConfigFileReader.h"
#include "simconfquery.h"
#include <fstream>
#include <iostream>

UDPConfigFileReader::UDPConfigFileReader() :
UDPConfigFileReader(0, std::string("/udp.conf")) {
}

UDPConfigFileReader::UDPConfigFileReader(unsigned int offset) :
UDPConfigFileReader(offset, std::string("/udp.conf")) {
}

UDPConfigFileReader::UDPConfigFileReader(std::string configfile) :
UDPConfigFileReader(0, configfile) {
}

UDPConfigFileReader::UDPConfigFileReader(unsigned int offset,
        std::string configfile) :
portOffset(offset) {
    // set default first
    targetIP = std::string("127.0.0.1");
    targetPort = 50000;
    localIP = std::string("127.0.0.1");
    localPort = 40000;

    std::ifstream configFile(configfile);
    if (configFile.is_open()) {
        std::string line;
        std::string token;

        // Who do I talk to?
        std::getline(configFile, line);
        token = line.substr(0, line.find(" ")); // ignore rest after IP-address
        targetIP = token;
        // Which port do I talk to?
        std::getline(configFile, line);
        token = line.substr(0, line.find(" ")); // ignore rest after port number
        targetPort = std::stoi(token);
        // What is my own address
        std::getline(configFile, line);
        token = line.substr(0, line.find(" ")); // ignore rest after IP-address
        localIP = token;
        // On which port should I listen?
        std::getline(configFile, line);
        token = line.substr(0, line.find(" ")); // ignore rest after port number
        localPort = std::stoi(token);

        // What is the reporting address
        std::string reportIP = std::string("127.0.0.1");
        uint16_t reportPort = (short) 60000;
        bool end_reached = false;
        while(!end_reached){
            std::getline(configFile, line);
            token = line.substr(0, line.find(" ")); // ignore rest after IP-address
            if (configFile.good()) {
                reportIP = token;
            } else {
                end_reached = true;
            }
            
            // On which port should I report?
            std::getline(configFile, line);
            token = line.substr(0, line.find(" ")); // ignore rest after port number
            if (configFile.good()) {
                reportPort = std::stoi(token);
                reportTargets.push_back(std::pair<std::string, uint16_t>(reportIP, reportPort));
            } else {
                end_reached = true;
            }
        }

        configFile.close();
    }
}

void UDPConfigFileReader::showConfig() {
    std::cout << "UDP-Target (send to): " << targetIP << ", "
            << targetPort + portOffset << std::endl;
    std::cout << "UDP-Local  (receive): " << localIP << ", "
            << localPort + portOffset << std::endl;
    for (std::pair<std::string, uint16_t> &e : reportTargets) {
        std::cout << "UDP-Report (send to): " << e.first << ", "
                << e.second + portOffset << std::endl;
    }
}

struct in_addr UDPConfigFileReader::getTargeHostIP() {
    struct in_addr addr;
    inet_aton(targetIP.c_str(), &addr);
    return addr;
}

uint16_t UDPConfigFileReader::getTargetHostPort() {
    return htons(targetPort + portOffset);
}

uint16_t UDPConfigFileReader::getLocalPort() {
    return htons(localPort + portOffset);
}

struct in_addr UDPConfigFileReader::getReportHostIP() {
    report_target_index = 0;   // reset list iteration
    struct in_addr addr = {0};
    if (reportTargets.size() > 0) {
        std::pair<std::string, uint16_t> element = reportTargets[0];
        std::string reportIP = element.first;
        //std::cout << reportIP << std::endl;
        inet_aton(reportIP.c_str(), &addr);
        //std::cout << addr.s_addr << std::endl;
    }
    return addr;
}

uint16_t UDPConfigFileReader::getReportHostPort() {
    uint16_t reportPort = 0;
    if (reportTargets.size() > 0) {
        std::pair<std::string, uint16_t> element = reportTargets[0];
        reportPort = element.second;
        //std::cout << reportPort << std::endl;
        reportPort = htons(reportPort + portOffset);
    }
    return reportPort;
}

unsigned int UDPConfigFileReader::getNumberReportHosts(){
    return reportTargets.size();
};

struct in_addr UDPConfigFileReader::getNextReportHostIP() {
    report_target_index ++;
    //std::cout << report_target_index << " " << reportTargets.size() << std::endl;
    struct in_addr addr = {0};
    if ((reportTargets.size() > 0) && (report_target_index < reportTargets.size())) {
        std::pair<std::string, uint16_t> element = reportTargets[report_target_index];
        std::string reportIP = element.first;
        //std::cout << reportIP << std::endl;
        inet_aton(reportIP.c_str(), &addr);
        //std::cout << addr.s_addr << std::endl;
    }
    return addr;
}; 

uint16_t UDPConfigFileReader::getNextReportHostPort() {
    uint16_t reportPort = 0;
    if ((reportTargets.size() > 0) && (report_target_index < reportTargets.size())) {
        std::pair<std::string, uint16_t> element = reportTargets[report_target_index];
        reportPort = element.second;
        //std::cout << reportPort << std::endl;
        reportPort = htons(reportPort + portOffset);
        //std::cout << reportPort << std::endl;
    }
    return reportPort;

}; 
