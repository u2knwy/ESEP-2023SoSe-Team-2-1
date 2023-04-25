//
// Created by domxs on 14.04.2023.
//
// event_manager.cpp

#include <algorithm>
#include "eventmanager.h"

// Method for subscribing to events
void EventManager::subscribe(EventType eventType, EventCallback callback) {
    subscriptions_[eventType].push_back(std::move(callback));
}

// Method for unsubscribing from events
void EventManager::unsubscribe(EventType eventType, EventCallback callback) {
    auto& subscribers = subscriptions_[eventType];
    subscribers.erase(
            std::remove_if(subscribers.begin(), subscribers.end(),
                           [&](const EventCallback& cb) { return cb.target<void(const Event&)>() == callback.target<void(const Event&)>(); }),
            subscribers.end());
}

// Method for unsubscribing from events
void EventManager::unsubscribeAll(void *objPointer) {
    // todo all unsubscribe


}


// Method for notifying subscribers when events occur
void EventManager::notify(const Event& event) {
    const auto& eventType = event.getType();
    if (subscriptions_.count(eventType) > 0) {
        const auto& subscribers = subscriptions_[eventType];
        for (const auto& callback : subscribers) {
            callback(event);
        }
    }
}
