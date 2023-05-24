/*
 * WorkpieceManager.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include <data/WorkpieceManager.h>

WorkpieceManager::WorkpieceManager() {
	nextId = 1;
	workpieceOnFBM2 = NULL;
}

WorkpieceManager::~WorkpieceManager() {
	// TODO Auto-generated destructor stub
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
