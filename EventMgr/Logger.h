//
// Created by domxs on 14.04.2023.
//

#ifndef CONSTRUCT_LOGGER_H
#define CONSTRUCT_LOGGER_H

#include <stdio.h>
#include "eventmanager.h"

class Logger {

private:
    EventManager eventManager;
    void *objPtr;

    void log(const Event &event) {
        // todo: log event

    }

public:
    Logger(Eventmanager &instance) {
        eventManager = instance;
        printf("Logger instance has been created \n");
    }

    void subscribeto(EventType event) {
        eventManager->subscribe(event, [&](const Event &event) { log(event); });
    }
    void unsubscribeAll(void obj*){
        // unsub to all
        printf("unsubscribed from all\n");
    }



    virtual ~Logger() {
        eventManager->unsubscribeAll(objPtr);
        printf("destructor called \n");
    }

};


#endif //CONSTRUCT_LOGGER_H
