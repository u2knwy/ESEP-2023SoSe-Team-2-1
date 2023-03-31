/* Simple demostration of the simulator usage
 * @author: Franz Korf
 * @author: Thomas Lehmann
 * @date: 2020-04-24
 */
#include <iostream>

/* Für die Simulation: */
// #include "../simulationqnx/simqnxgpioapi.h" // must be last include !!!

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
        usleep(1000*500);

        hal->GreenLampOff();
        usleep(1000*500);
	}

	return 0;
}
