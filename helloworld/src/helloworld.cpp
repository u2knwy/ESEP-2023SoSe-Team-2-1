/* Simple demostration of the simulator usage
 * @author: Franz Korf
 * @author: Thomas Lehmann
 * @date: 2020-04-24
 */
#include <iostream>

/* Für die Simulation: */
// #include "../simulationqnx/simqnxgpioapi.h" // must be last include !!!

/* Für die Hardware: */
#include <sys/mman.h>
#include <hw/inout.h>

using namespace std;

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

		out32((uintptr_t) (gpio_bank_1 + 0x190), 0x00010000); // clear register
        usleep(1000*500);

		out32((uintptr_t) (gpio_bank_1 + 0x190), 0x00020000); // clear register
        usleep(1000*500);

	}

	return 0;
}
