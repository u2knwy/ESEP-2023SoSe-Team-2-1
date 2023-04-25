#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Watchdog.h"
#include "Logger.h"
#include "eventmanager.h"
#include "errno.h"

void heartBeatCallback(const Event &event) {
    // Get event data
    int intValue = event.getIntValue();
    const std::string &stringValue = event.getStringValue();

    // Process the event data
    std::cout << "HeartBeat received with int value: " << intValue << std::endl;
    std::cout << "HeartBeat received with string value: " << stringValue << std::endl;
}

int main() {
    printf("hello main \n");

    // Create an instance of the EventManager
    EventManager eventManager;
    Logger logobj(eventManager);
    // Subscribe to the ButtonClicked event
    eventManager.subscribe(EventType::HeartBeat, heartBeatCallback);
    // Create an event with int and string data
    int intValue = 505;
    std::string stringValue = "Heartbeat sent";
    Event event(EventType::HeartBeat, intValue, stringValue);
    // Notify subscribers of the ButtonClicked event
    eventManager.notify(event);

    return EXIT_SUCCESS;
}
