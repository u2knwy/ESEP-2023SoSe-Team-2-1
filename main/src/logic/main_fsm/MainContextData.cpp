/*
 * MainContextData.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "MainContextData.h"
#include "logger/logger.hpp"
#include "configuration/Configuration.h"

MainContextData::MainContextData() {
    wpManager = new WorkpieceManager();
    setRampFBM1Blocked(false);
    setRampFBM2Blocked(false);
    bool isMaster = Configuration::getInstance().systemIsMaster();
    if(isMaster) {
        master_pusherMounted = Configuration::getInstance().pusherMounted();
    	slave_pusherMounted = false;
    } else {
		master_pusherMounted = false;
		slave_pusherMounted = Configuration::getInstance().pusherMounted();
    }
}

MainContextData::~MainContextData() { delete wpManager; }

void MainContextData::setRampFBM1Blocked(bool blocked) {
    this->rampFBM1Blocked = blocked;
    wpManager->setRamp_one(blocked);
}

void MainContextData::setRampFBM2Blocked(bool blocked) {
    this->rampFBM2Blocked = blocked;
    wpManager->setRamp_two(blocked);
}

bool MainContextData::isRampFBM1Blocked() { return wpManager->getRamp_one(); }

bool MainContextData::isRampFBM2Blocked() { return wpManager->getRamp_two(); }


void MainContextData::resetSelftestSensorsResult() {
	ssResult.master_lbStartOk = false;
	ssResult.master_lbSwitchOk = false;
	ssResult.master_lbRampOk = false;
	ssResult.master_lbEndOk = false;
	ssResult.slave_lbStartOk = false;
	ssResult.slave_lbSwitchOk = false;
	ssResult.slave_lbRampOk = false;
	ssResult.slave_lbEndOk = false;
}

bool MainContextData::getSelftestSensorsResult() {
	if (!ssResult.master_lbStartOk) {
	        Logger::error("[Selftest] LB Start at FBM1 not ok");
	    }
	    if (!ssResult.master_lbSwitchOk) {
	        Logger::error("[Selftest] LB Switch at FBM1 not ok");
	    }
	    if (!ssResult.master_lbRampOk) {
	        Logger::error("[Selftest] LB Ramp at FBM1 not ok");
	    }
	    if (!ssResult.master_lbEndOk) {
	        Logger::error("[Selftest] LB End at FBM1 not ok");
	    }
	    if (!ssResult.slave_lbStartOk) {
	        Logger::error("[Selftest] LB Start at FBM2 not ok");
	    }
	    if (!ssResult.slave_lbSwitchOk) {
	        Logger::error("[Selftest] LB Switch at FBM2 not ok");
	    }
	    if (!ssResult.slave_lbRampOk) {
	        Logger::error("[Selftest] LB Ramp at FBM2 not ok");
	    }
	    if (!ssResult.slave_lbEndOk) {
	        Logger::error("[Selftest] LB End at FBM2 not ok");
	    }

	    return ssResult.master_lbStartOk && ssResult.master_lbSwitchOk && ssResult.master_lbRampOk &&
	    		ssResult.master_lbEndOk && ssResult.slave_lbStartOk && ssResult.slave_lbSwitchOk &&
				ssResult.slave_lbRampOk && ssResult.slave_lbEndOk;
}
