/*
 * MainContextData.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "MainContextData.h"

MainContextData::MainContextData() {
	wpManager = new WorkpieceManager();
}

MainContextData::~MainContextData() {
	delete wpManager;
}

void MainContextData::setRampFBM1Blocked(bool blocked) {
	this->rampFBM1Blocked = blocked;
}

void MainContextData::setRampFBM2Blocked(bool blocked) {
	this->rampFBM2Blocked = blocked;
}

bool MainContextData::isRampFBM1Blocked() {
	return this->rampFBM1Blocked;
}

bool MainContextData::isRampFBM2Blocked() {
	return this->rampFBM2Blocked;
}
