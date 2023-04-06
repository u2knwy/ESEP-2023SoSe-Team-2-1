/* 
 * File:   simitemhandling.cpp
 * @author Lehmann
 * @date 3. April 2020
 */

#include "simitemhandling.h"

SimItemHandling::SimItemHandling() :
        manager(nullptr) {
}

SimItemHandling::SimItemHandling(SimItemManager* manager) :
        manager(manager) {
}

void SimItemHandling::init() {
    std::lock_guard<std::mutex> lk(buffermutex);
    for (auto &it : actions) { // it is reference to action...
        it.executed = false;
    }
}

void SimItemHandling::evalTime(unsigned int simTime) {
    // find all actions that have not been executed and have less or equal time as the simtime
    std::lock_guard<std::mutex> lk(buffermutex);
    for (auto &it : actions) { // it is reference to action...
        if (!it.executed) {
            if (it.atTime <= simTime) {
                if (manager != nullptr) {
                    manager->handleItemAction(it);
                }
                it.executed = true;
            }
        }
    }
}

void SimItemHandling::addAction(const SimItemHandlingAction &action) {
    std::lock_guard<std::mutex> lk(buffermutex);
    actions.push_back(action);
}
