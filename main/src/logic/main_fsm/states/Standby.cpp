/*
 * Standby.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "Standby.h"
#include <iostream>

using namespace std;

void Standby::entry() {
	cout << "Standby::entry" << endl;
}

void Standby::exit() {
	cout << "Standby::exit" << endl;
}

bool Standby::master_btnStart_PressedShort() {
	cout << "Standby::master_btnStart_PressedShort" << endl;
	return true;
}

bool Standby::master_btnStart_PressedLong() {
	cout << "Standby::master_btnStart_PressedLong" << endl;
	return true;
}

bool Standby::master_EStop_Pressed() {
	cout << "Standby::master_EStop_Pressed" << endl;
	return true;
}

bool Standby::slave_btnStart_PressedShort() {
	cout << "Standby::slave_btnStart_PressedShort" << endl;
	return true;
}

bool Standby::slave_btnStart_PressedLong() {
	cout << "Standby::slave_btnStart_PressedLong" << endl;
	return true;
}

bool Standby::slave_EStop_Pressed() {
	cout << "Standby::slave_EStop_Pressed" << endl;
	return true;
}

