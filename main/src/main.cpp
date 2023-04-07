#include <iostream>
#include "hal/hal.h"

using namespace std;

int main() {
	cout << "Switch traffic light on / off" << endl;

	// Create a shared pointer which holds a reference to the HAL.
	// This can be shared across multiple classes and the destructor will automatically be called,
	// if there are no more references to it.
	shared_ptr<HAL> hal = std::make_shared<HAL>();

#define RUNS 10
	for(int i = 0; i < RUNS; i++){
		cout << "loop " << i+1 << "/" << RUNS << endl;
		hal->GreenLampOn();
		this_thread::sleep_for(chrono::seconds(1));

        hal->GreenLampOff();
        this_thread::sleep_for(chrono::seconds(1));
	}

	return 0;
}
