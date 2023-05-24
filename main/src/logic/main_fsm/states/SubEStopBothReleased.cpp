/*
 * SubEStopTwoPressed.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "SubEStopBothReleased.h"
#include "SubEStopOnePressed.h"
#include "Standby.h"

#include <iostream>

using namespace std;

void SubEStopBothReleased::entry() {
	cout << "SubEStopBothReleased::entry" << endl;
	masterReset = false;
	slaveReset = false;
}

bool SubEStopBothReleased::master_btnReset_Pressed() {
	cout << "SubEStopBothReleased::master_btnReset_Pressed" << endl;
	this->masterReset = true;
	if(masterReset && slaveReset) {
		exit();
		new(this) Standby;
		entry();
	}
	return true;
}

bool SubEStopBothReleased::slave_btnReset_Pressed() {
	cout << "SubEStopBothReleased::slave_btnReset_Pressed" << endl;
	this->slaveReset = true;
	if(masterReset && slaveReset) {
		exit();
		new(this) Standby;
		entry();
	}
	return true;
}

bool SubEStopBothReleased::master_EStop_Pressed() {
	cout << "SubEStopBothReleased::master_EStop_Pressed" << endl;
	exit();
	new(this) SubEStopOnePressed;
	entry();
	return true;
}

bool SubEStopBothReleased::slave_EStop_Pressed() {
	cout << "SubEStopBothReleased::slave_EStop_Pressed" << endl;
	exit();
	new(this) SubEStopOnePressed;
	entry();
	return true;
}
