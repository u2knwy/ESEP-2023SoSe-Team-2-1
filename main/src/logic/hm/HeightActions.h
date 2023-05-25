/*
 * HeightActions.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#ifndef SRC_LOGIC_HM_HEIGHTACTIONS_H_
#define SRC_LOGIC_HM_HEIGHTACTIONS_H_

#include "events/eventtypes_enum.h"
#include "configuration/Configuration.h"

class HeightActions {
public:
	HeightActions();
	virtual ~HeightActions();
	void newWorkpieceDetected();
	void sendHeightResultFBM1(WorkpieceType type, int avg);
	void sendHeightResultFBM2(WorkpieceType type, int max);
};

#endif /* SRC_LOGIC_HM_HEIGHTACTIONS_H_ */
