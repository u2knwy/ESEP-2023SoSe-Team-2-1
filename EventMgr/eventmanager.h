#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include "msg_enum.h"

// Define the Event class
class Event {
public:
    // Constructor
    Event(EventType type, int intValue, const std::string &stringValue)
            : type_(type), intValue_(intValue), stringValue_(stringValue) {}

    // Getters for event data
    EventType getType() const { return type_; }

    int getIntValue() const { return intValue_; }

    const std::string &getStringValue() const { return stringValue_; }

private:
    // Event data
    EventType type_;
    int intValue_;
    std::string stringValue_;
};

// Define the EventManager class
class EventManager {
public:
    // Type for callback function
    using EventCallback = std::function<void(const Event &)>;

    // Methods for subscribing/unsubscribing to events
    void subscribe(EventType eventType, EventCallback callback);

    void unsubscribe(EventType eventType, EventCallback callback);

    void unsubscribeAll(void *objPointer);

    // Method for notifying subscribers when events occur
    void notify(const Event &event);

private:
    // Data structure for storing event subscriptions
    std::unordered_map<EventType, std::vector<EventCallback>> subscriptions_;
};

