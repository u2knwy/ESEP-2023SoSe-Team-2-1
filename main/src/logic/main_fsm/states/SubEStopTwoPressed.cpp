/*
 * SubEStopTwoPressed.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "SubEStopTwoPressed.h"
#include "SubEStopOnePressed.h"

#include <iostream>

using namespace std;

bool SubEStopTwoPressed::master_EStop_Released() {
	cout << "SubEStopTwoPressed::master_EStop_Released" << endl;
	exit();
	new(this) SubEStopOnePressed;
	entry();
	return true;
}

bool SubEStopTwoPressed::slave_EStop_Released() {
	cout << "SubEStopTwoPressed::slave_EStop_Released" << endl;
	exit();
	new(this) SubEStopOnePressed;
	entry();
	return true;
}
