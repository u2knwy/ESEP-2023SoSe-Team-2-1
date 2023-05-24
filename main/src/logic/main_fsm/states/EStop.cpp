/*
 * EStop.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "EStop.h"
#include <iostream>

using namespace std;

void EStop::entry() {
	cout << "EStop::entry" << endl;
}

void EStop::exit() {
	cout << "EStop::exit" << endl;
}

bool EStop::master_EStop_Pressed() {
	cout << "EStop::master_EStop_Pressed" << endl;
	return true;
}

bool EStop::master_EStop_Released() {
	cout << "EStop::master_EStop_Released" << endl;
	return true;
}

bool EStop::slave_EStop_Pressed() {
	cout << "EStop::slave_EStop_Pressed" << endl;
	return true;
}

bool EStop::slave_EStop_Released() {
	cout << "EStop::slave_EStop_Released" << endl;
	return true;
}

bool EStop::master_btnReset_Pressed() {
	cout << "EStop::master_btnReset_Pressed" << endl;
	return true;
}

bool EStop::slave_btnReset_Pressed() {
	cout << "EStop::slave_btnReset_Pressed" << endl;
	return true;
}
