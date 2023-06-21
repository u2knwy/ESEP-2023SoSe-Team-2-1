/*
 * MainContextData.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "MainContextData.h"

MainContextData::MainContextData() {
    wpManager = new WorkpieceManager();
    setRampFBM1Blocked(false);
    setRampFBM2Blocked(false);
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

bool MainContextData::isRampFBM1Blocked() { return this->rampFBM1Blocked; }

bool MainContextData::isRampFBM2Blocked() { return this->rampFBM2Blocked; }
