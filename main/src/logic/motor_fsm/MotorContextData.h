/*
 * MotorContextData.h
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */
#pragma once

class MotorContextData {
public:
	MotorContextData();
	virtual ~MotorContextData();
	void setStopFlag(bool stop);
	void setSlowFlag(bool slow);
	void setRightFlag(bool right);
	bool getStop();
	bool getRight();
	bool getSlow();
private:
	bool stop;
	bool right;
	bool slow;
};
