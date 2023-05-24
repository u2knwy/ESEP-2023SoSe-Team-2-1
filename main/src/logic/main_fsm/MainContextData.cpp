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

