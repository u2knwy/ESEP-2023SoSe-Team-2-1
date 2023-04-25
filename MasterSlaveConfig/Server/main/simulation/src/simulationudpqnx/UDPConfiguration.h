/*
 * UDPConfiguration.h
 *
 *  Created on: 25.06.2019
 *      Author: Lehmann
 */

#ifndef UDP_CONFIGURATION_H_
#define UDP_CONFIGURATION_H_

extern "C" {
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
}
#include <stdint.h>

class UDPConfiguration {
public:
    virtual ~UDPConfiguration() {};
    virtual struct in_addr getTargeHostIP() = 0; // Target address in network byte order
    virtual uint16_t getTargetHostPort() = 0; // Port in network byte order
    virtual uint16_t getLocalPort() = 0; // Receiving port in network byte order
};

class UDPConfigurationStatic : public UDPConfiguration {
private:
    unsigned int portOffset;
public:
    UDPConfigurationStatic();
    UDPConfigurationStatic(unsigned int offset);
    virtual ~UDPConfigurationStatic() {};
    virtual struct in_addr getTargeHostIP(); // Target address in network byte order
    virtual uint16_t getTargetHostPort(); // Port in network byte order
    virtual uint16_t getLocalPort(); // Receiving port in network byte order
};

#endif /* UDP_CONFIGURATION_H_ */
