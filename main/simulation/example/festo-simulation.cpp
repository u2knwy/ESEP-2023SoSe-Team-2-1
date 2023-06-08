/*
 * festo-simulation.cpp
 *
 *  Created on: 06.04.2023
 *      Author: Maik
 */
#include <iostream>

#include "simqnxgpioapi.h"

using namespace std;

/*
int main() {
	cout << "Switch traffic light on / off" << endl;

	uintptr_t gpio_bank_1 = mmap_device_io(0x1000,(uint64_t) 0x4804C000);
	while (1) {
		// very dirty
		// register description:
		// spruh73l.pdf page 4877
		out32((uintptr_t) (gpio_bank_1 + 0x194), 0x00010000); // set register
        usleep(1000*500);

		out32((uintptr_t) (gpio_bank_1 + 0x194), 0x00020000); // set register
        usleep(1000*500);

		out32((uintptr_t) (gpio_bank_1 + 0x194), 0x00040000); // set register
        usleep(1000*1000);

		out32((uintptr_t) (gpio_bank_1 + 0x190), 0x00010000); // clear register
        usleep(1000*500);

		out32((uintptr_t) (gpio_bank_1 + 0x190), 0x00020000); // clear register
        usleep(1000*500);

		out32((uintptr_t) (gpio_bank_1 + 0x190), 0x00040000); // clear register
        usleep(1000*1000);
	}

	return 0;
}
*/
