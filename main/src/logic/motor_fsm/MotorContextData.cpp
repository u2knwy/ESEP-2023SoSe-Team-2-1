/*
 * MotorContextData.cpp
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */

#include <logic/motor_fsm/MotorContextData.h>

MotorContextData::MotorContextData() {
	stop = false;
	right = false;
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

void MotorContextData::setRightFlag(bool right) {
	this->right = right;
}

bool MotorContextData::getStop() {
	return stop;
}

bool MotorContextData::getRight() {
	return right;
}

bool MotorContextData::getSlow() {
	return slow;
}
