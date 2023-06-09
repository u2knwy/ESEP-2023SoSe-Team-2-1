/*
 * hal.cpp
 *
 *  Created on: 04.04.2023
 *      Author: Maik
 */
#include <gtest/gtest.h>
#include "hal/hal.h"
#include "hal/HeightSensor.h"
#include "hal/Actuators.h"
#include "hal/Sensors.h"
#include "events/EventManager.h"

class HAL_Test : public ::testing::Test
{
protected:
	/*	std::shared_ptr<EventManager> mngr;
		//Actuators* actuators;
		Sensors* sensors;
		std::shared_ptr<HeightSensorFSM> hmFSM;
		HeightSensor* hm;

		void SetUp() override {
			mngr = std::make_shared<EventManager>();
			hmFSM = std::make_shared<HeightSensorFSM>();
			hm = new HeightSensor(hmFSM);
		}

		void TearDown() override {
			//delete actuators;
			delete sensors;
			delete hm;
		}*/
};

static bool readPin(uint32_t port, uint32_t pin)
{
	uintptr_t gpio_bank;
	switch (port)
	{
	case 0:
		gpio_bank = mmap_device_io(SIZE_4KB, (uint64_t)GPIO_BANK_0);
		break;
	case 1:
		gpio_bank = mmap_device_io(SIZE_4KB, (uint64_t)GPIO_BANK_1);
		break;
	case 2:
		gpio_bank = mmap_device_io(SIZE_4KB, (uint64_t)GPIO_BANK_2);
		break;
	default:
		perror("Invalid parameter 'port' for 'readPin' function");
		exit(EXIT_FAILURE);
	}
	uint32_t reg = (in32(GPIO_DATAIN(gpio_bank)) >> pin);
	std::cout << "REG: " << std::bitset<32>(reg) << std::endl;
	bool result = (reg & 0x1) == 0x1;
	munmap_device_io(gpio_bank, SIZE_4KB);
	return result;
}

/*TEST_F(HAL_Test, GreenLampOn) {
	std::shared_ptr<EventManager> mngr = std::make_shared<EventManager>();
	Actuators* actuators = new Actuators(mngr);
	actuators->greenLampOn();
	delete actuators;
	EXPECT_EQ(true, readPin(1, 18));
}

TEST_F(HAL_Test, GreenLampOff) {
	std::shared_ptr<EventManager> mngr = std::make_shared<EventManager>();
	Actuators* actuators = new Actuators(mngr);
	actuators->greenLampOff();
	delete actuators;
	EXPECT_EQ(false, readPin(1, 18));
}*/
