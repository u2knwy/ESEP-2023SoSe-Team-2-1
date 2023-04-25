/* 
 * File:   simhci.cpp
 * @author Lehmann
 * @date 29. März 2020
 */

#include "simhci.h"
#include "simmasks.h"
#include "simconfquery.h"
#include <iostream>

using namespace std;

SimHCI::SimHCI(SimulationIOImage* shadow) : shadow(shadow) {
};

void SimHCI::init() {
    unsigned short button_mask = SIM_BUTTON_START | SIM_BUTTON_STOP | SIM_BUTTON_RESET | SIM_EMERGENCY_STOP;
    unsigned short action = SIM_EMERGENCY_STOP | SIM_BUTTON_STOP;
    if (shadow != nullptr) {
        shadow->in = (shadow->in & ~button_mask) | action;
    }
    {
        std::lock_guard<std::mutex> lk(buffermutex);

        for (auto &it : actions) { // it is reference to action...
            it.executed = false;
        }
    }
}

void SimHCI::evalTime(unsigned int simTime) {
    std::lock_guard<std::mutex> lk(buffermutex);
    unsigned short button_mask = SIM_BUTTON_START | SIM_BUTTON_STOP | SIM_BUTTON_RESET | SIM_EMERGENCY_STOP;

    //cout << "simTime" << simTime << endl;

    for (auto &it : actions) { // it is reference to action...
        if (!it.executed) {
            if (it.atTime <= simTime) {
                if (shadow != nullptr) {
                    shadow->in = (shadow->in & ~button_mask) | it.pattern;
                    if(SIMCONFQUERRY_ISACTIVE(showactions)){
                    	cout << "<SIM> new HCI ACTION: " << "0x" << hex << it.pattern << dec << endl;
                    }
                }
                it.executed = true;
            }
        }
    }
}

void SimHCI::addAction(const SimHCIAction &action) {
    std::lock_guard<std::mutex> lk(buffermutex);
    actions.push_back(action);
}

