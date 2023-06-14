/*
 * WorkpieceManager.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include <data/WorkpieceManager.h>
#include "configuration/Configuration.h"

WorkpieceManager::WorkpieceManager() {
	nextId = 1;
	workpieceOnFBM2 = NULL;

	auto confOrder = Configuration::getInstance().getDesiredOrder();
	for(int i = 0; i < 3; i++) {
		desiredOrder[i] = confOrder.at(i);
	}
}

WorkpieceManager::~WorkpieceManager() {
	delete workpieceOnFBM2;
}

void WorkpieceManager::rotateNextWorkpieces() {
	WorkpieceType front = desiredOrder[0];
	// 2nd -> 1st
	desiredOrder[0] = desiredOrder[1];
	// 3rd -> 2nd
	desiredOrder[1] = desiredOrder[2];
	// 1st -> 3rd
	desiredOrder[2] = front;
}

WorkpieceType WorkpieceManager::getNextWorkpieceType() {
	return desiredOrder[0];
}

Workpiece* WorkpieceManager::addWorkpiece() {
	Workpiece* wp = new Workpiece(nextId);
	this->workpiecesOnFBM1.push_back(wp);
	nextId++;
	return wp;
}

int WorkpieceManager::nWorkpiecesOnFBM1() {
	return this->workpiecesOnFBM1.size();
}

bool WorkpieceManager::isFBM2Occupied() {
	return this->workpieceOnFBM2 != NULL;
}

bool WorkpieceManager::removeWpOnFBM2() {
	if (this->workpieceOnFBM2 != NULL) {
		return false;
	} else {
		this->workpieceOnFBM2 = NULL;
		return true;
	}
}

bool WorkpieceManager::wpTransferredToFBM2() {
	if (this->workpieceOnFBM2 != NULL) {
		return false;
	} else {
		Workpiece* wp = workpiecesOnFBM1.front();
		workpiecesOnFBM1.erase(workpiecesOnFBM1.begin());
		workpieceOnFBM2 = wp;
		return true;
	}
}
