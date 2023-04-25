/* 
 * File:   simctrlhandler.cpp
 * @author Lehmann
 * @date 16. Mai 2020
 */

#include "simctrlhandler.h"
#include "simctrlaction.h"

void SimCtrlHandler::addAction(SimCtrlAction action) {
    // execute only on action at a time
    std::lock_guard<std::mutex> lk(buffermutex);

    if (sim != nullptr) {
        if (action.isCommandStart()) {
            sim->releaseSimulation();
        }
    }
}
