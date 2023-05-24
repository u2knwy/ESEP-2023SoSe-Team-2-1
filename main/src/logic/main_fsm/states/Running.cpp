/*
 * Running.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "Running.h"
#include "Standby.h"

#include <iostream>

using namespace std;

void Running::entry() {
	cout << "Running::entry" << endl;
}

void Running::exit() {
	cout << "Running::exit" << endl;
}

bool Running::master_LBA_Blocked() {
	cout << "Running::master_LBA_Blocked" << endl;
	return true;
}

bool Running::master_LBA_Unblocked() {
	cout << "Running::master_LBA_Unblocked" << endl;
	return true;
}

bool Running::master_LBW_Blocked() {
	cout << "Running::master_LBW_Blocked" << endl;
	return true;
}

bool Running::master_LBW_Unblocked() {
	cout << "Running::master_LBW_Unblocked" << endl;
	return true;
}

bool Running::master_LBE_Blocked() {
	cout << "Running::master_LBE_Blocked" << endl;
	return true;
}

bool Running::master_LBE_Unblocked() {
	cout << "Running::master_LBE_Unblocked" << endl;
	return true;
}

bool Running::master_LBR_Blocked() {
	cout << "Running::master_LBR_Blocked" << endl;
	return true;
}

bool Running::master_LBR_Unblocked() {
	cout << "Running::master_LBR_Unblocked" << endl;
	return true;
}

bool Running::slave_LBA_Blocked() {
	cout << "Running::slave_LBA_Blocked" << endl;
	return true;
}

bool Running::slave_LBA_Unblocked() {
	cout << "Running::slave_LBA_Unblocked" << endl;
	return true;
}

bool Running::slave_LBW_Blocked() {
	cout << "Running::slave_LBW_Blocked" << endl;
	return true;
}

bool Running::slave_LBW_Unblocked() {
	cout << "Running::slave_LBW_Unblocked" << endl;
	return true;
}

bool Running::slave_LBE_Blocked() {
	cout << "Running::slave_LBE_Blocked" << endl;
	return true;
}

bool Running::slave_LBE_Unblocked() {
	cout << "Running::slave_LBE_Unblocked" << endl;
	return true;
}

bool Running::slave_LBR_Blocked() {
	cout << "Running::slave_LBR_Blocked" << endl;
	return true;
}

bool Running::slave_LBR_Unblocked() {
	cout << "Running::slave_LBR_Unblocked" << endl;
	return true;
}

bool Running::master_btnStop_Pressed() {
	cout << "Running::master_btnStop_Pressed" << endl;
	exit();
	new(this) Standby;
	entry();
	return true;
}

bool Running::slave_btnStop_Pressed() {
	cout << "Running::slave_btnStop_Pressed" << endl;
	exit();
	new(this) Standby;
	entry();
	return true;
}

bool Running::master_EStop_Pressed() {
	cout << "Running::master_EStop_Pressed" << endl;
	return true;
}

bool Running::slave_EStop_Pressed() {
	cout << "Running::slave_EStop_Pressed" << endl;
	return true;
}
