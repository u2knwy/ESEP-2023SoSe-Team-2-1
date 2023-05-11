/*
 * HeightActions.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#ifndef SRC_LOGIC_HM_HEIGHTACTIONS_H_
#define SRC_LOGIC_HM_HEIGHTACTIONS_H_

#include "events/eventtypes_enum.h"

class HeightActions {
public:
	HeightActions();
	virtual ~HeightActions();
	void sendHeightResultFBM1(EventType type, int avg);
	void sendHeightResultFBM2(EventType type, int max);
};

#endif /* SRC_LOGIC_HM_HEIGHTACTIONS_H_ */
