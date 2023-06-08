/* 
 * File:   simconfhandler.cpp
 * @author Lehmann
 * @date 08. Feb 2023
 */

#include "simconfhandler.h"
#include "simconf.h"
#include "simconfquery.h"

#include <iostream>

using namespace std;

SimConfHandler::SimConfHandler() {
};

void SimConfHandler::init() {
    {
        std::lock_guard<std::mutex> lk(buffermutex);

        for (auto &it : actions) { // it is reference to action...
            it.executed = false;
        }
    }
}

void SimConfHandler::evalTime(unsigned int simTime) {
    std::lock_guard<std::mutex> lk(buffermutex);

    //cout << "simTime" << simTime << endl;

    for (auto &it : actions) { // it is reference to action...
        if (!it.executed) {
            if (it.atTime <= simTime) {
            	// Eval
            	if(it.value2configure){
            		SimConfiguration::getInstance()->activate(it.parameter2configure);
            	} else {
            		SimConfiguration::getInstance()->deactivate(it.parameter2configure);
            	}
                if(SIMCONFQUERRY_ISACTIVE(showactions)){
                    cout << "<SIM> new Configuration ACTION: " << (SimConfiguration::SimConfStrings[static_cast<int>(it.parameter2configure)]) << " set to " << it.value2configure << endl;
                }
                it.executed = true;
            }
        }
    }
}

void SimConfHandler::addAction(const SimConfAction &action) {
    std::lock_guard<std::mutex> lk(buffermutex);
    if(action.atTime == 0){
    	// execute immediately
		if(action.value2configure){
			SimConfiguration::getInstance()->activate(action.parameter2configure);
		} else {
			SimConfiguration::getInstance()->deactivate(action.parameter2configure);
		}
    } else {
    	actions.push_back(action);
    }
}

