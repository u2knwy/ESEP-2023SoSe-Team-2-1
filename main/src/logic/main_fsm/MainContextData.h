/*
 * MainContextData.h
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */
#pragma once
#include "data/WorkpieceManager.h"

class MainContextData {
public:
	MainContextData();
	virtual ~MainContextData();
	WorkpieceManager* wpManager;
};
