/*
 * hal.cpp
 *
 *  Created on: 31.03.2023
 *      Author: Maik
 */

#include "hal.h"

#include <iostream>

HAL::HAL() {
	std::cout << "Create HAL" << std::endl;
	gpio_bank_1 = mmap_device_io(SIZE, (uint64_t) GPIO_BANK_1);
}

HAL::~HAL() {
	std::cout << "Destroy HAL" << std::endl;
	munmap_device_io(gpio_bank_1, SIZE);
}

void HAL::GreenLampOn() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), LAMP_GREEN_PIN); // set register
}

void HAL::GreenLampOff() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), LAMP_GREEN_PIN); // clear register
}
