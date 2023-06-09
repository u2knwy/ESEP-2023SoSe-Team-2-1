/*
 * MotorContextData.cpp
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */

#include <logic/motor_fsm/MotorContextData.h>

MotorContextData::MotorContextData() {
	stop = false;
	fast = false;
	slow = false;
}

MotorContextData::~MotorContextData() {
	// TODO Auto-generated destructor stub
}

void MotorContextData::setStopFlag(bool stop) {
	this->stop = stop;
}

void MotorContextData::setSlowFlag(bool slow) {
	this->slow = slow;
}

void MotorContextData::setFastFlag(bool fast) {
	this->fast = fast;
}

bool MotorContextData::getStop() {
	return stop;
}

bool MotorContextData::getFast() {
	return fast;
}

bool MotorContextData::getSlow() {
	return slow;
}
