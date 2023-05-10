#pragma once

#include <functional>
#include <map>
#include <vector>
#include <string>

#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include "../events/eventtypes_enum.h"


#define NO_OF_MSGS 6
#define ATTACH_POINT "myservice"
#define STR_MSG (_IO_MAX + 1)
#define DATA_MSG (_IO_MAX + 2)

using namespace std;

//std::shared_ptr<HAL> hal;
typedef struct _pulse header_t;
/* Second header: used by application - if required */
typedef struct
{
	int size;  // size of data block
	int count;
	EventType eventnr;// some counter used by the application
			   // further data fields required by our application
} app_header_t;

// Define the EventData class
class EventData {
public:
    // Constructor
    EventData(EventType type, int intValue, const std::string &stringValue)
            : type_(type), intValue_(intValue), stringValue_(stringValue) {}

    // Getters for event data
    EventType getType() const { return type_; }

    int getIntValue() const { return intValue_; }

    const std::string &getStringValue() const { return stringValue_; }

private:
    // EventData data
    EventType type_;
    int intValue_;
    std::string stringValue_;
};

// Define the EventManager class
class EventManager {
public:
    // Type for callback function
    using EventCallback = std::function<void(const EventData &)>;

    // Methods for subscribing/unsubscribing to events
    void subscribe(EventType eventType, EventCallback callback);

    void unsubscribe(EventType eventType, EventCallback callback);

    void unsubscribeAll(void *objPointer); // todo this function should unsubscribe calling object from all subscriptions

    // Method for notifying subscribers when events occur
    void sendEvent(const EventData &event);

    int start();
private:
    // Data structure for storing event subscriptions
    std::map<EventType, std::vector<EventCallback>> subscriptions_;
    void handle_app_msg(header_t hdr, int rcvid);
    void handle_ONX_IO_msg(header_t hdr, int rcvid);
    void handle_pulse(header_t hdr, int rcvid);

    int chid = -1;
};

