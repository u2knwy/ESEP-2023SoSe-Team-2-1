/*
 * SubEStopOnePressed.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "SubEStopOnePressed.h"
#include "SubEStopTwoPressed.h"

#include <iostream>

using namespace std;

void SubEStopOnePressed::entry() {
	cout << "SubEStopOnePressed::entry" << endl;
}

void SubEStopOnePressed::exit() {
	cout << "SubEStopOnePressed::exit" << endl;
}

bool SubEStopOnePressed::master_EStop_Pressed() {
	cout << "SubEStopOnePressed::master_EStop_Pressed" << endl;
	exit();
	new(this) SubEStopTwoPressed;
	entry();
	return true;
}

bool SubEStopOnePressed::master_EStop_Released() {
	cout << "SubEStopOnePressed::master_EStop_Released" << endl;
	return true;
}

bool SubEStopOnePressed::slave_EStop_Pressed() {
	cout << "SubEStopOnePressed::slave_EStop_Pressed" << endl;
	exit();
	new(this) SubEStopTwoPressed;
	entry();
	return true;
}

bool SubEStopOnePressed::slave_EStop_Released() {
	cout << "SubEStopOnePressed::slave_EStop_Released" << endl;
	return true;
}
