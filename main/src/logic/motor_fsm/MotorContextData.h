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
	void setSlowFlag(bool stop);
	void setFastFlag(bool stop);
	bool getStop();
	bool getFast();
	bool getSlow();
private:
	bool stop;
	bool fast;
	bool slow;
};
