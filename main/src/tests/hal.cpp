/*
 * hal.cpp
 *
 *  Created on: 04.04.2023
 *      Author: Maik
 */
#include <gtest/gtest.h>
#include "hal/hal.h"
#include "hal/HeightSensor.h"
#include "events/EventManager.h"

class HAL_Test : public ::testing::Test {
protected:
  HAL* hal;
  HeightSensor* hm;

  void SetUp() override {
	  std::shared_ptr<EventManager> mngr = std::make_shared<EventManager>();
	  hal = new HAL(mngr);
	  auto fsm = std::make_shared<HeightSensorFSM>();
	  hm = new HeightSensor(fsm);
  }

  void TearDown() override {
	  delete hal;
	  delete hm;
  }
};

static bool readPin(uint32_t port, uint32_t pin) {
	uintptr_t gpio_bank;
	switch(port) {
	case 0:
		gpio_bank = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_0);
		break;
	case 1:
		gpio_bank = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_1);
		break;
	case 2:
		gpio_bank = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_2);
		break;
	default:
		perror("Invalid parameter 'port' for 'readPin' function");
		exit(EXIT_FAILURE);
	}
	bool result = ((in32((uintptr_t) gpio_bank + GPIO_DATAIN) >> pin) & 0x1) == 0x1;
	munmap_device_io(gpio_bank, GPIO_SIZE);
	return result;
}

TEST_F(HAL_Test, GreenLamp) {
	// Example: now we can call methods on the object created in SetUp method
	hal->GreenLampOn();
	EXPECT_EQ(true, readPin(1, 18));
	hal->GreenLampOff();
	EXPECT_EQ(false, readPin(1, 18));
}

TEST_F(HAL_Test, YellowLamp) {
	// Example: now we can call methods on the object created in SetUp method
	hal->YellowLampOn();
	EXPECT_EQ(true, readPin(1, 17));
	hal->YellowLampOff();
	EXPECT_EQ(false, readPin(1, 17));
}

TEST_F(HAL_Test, RedLamp) {
	// Example: now we can call methods on the object created in SetUp method
	hal->RedLampOn();
	EXPECT_EQ(true, readPin(1, 16));
	hal->RedLampOff();
	EXPECT_EQ(false, readPin(1, 16));
}

TEST_F(HAL_Test, HmFlat) {
	hm->calibrateOffset(3500);
	hm->calibrateRefHigh(3000);
	// 3500-3000 = 500
	// 500/25mm = 20 inc/mm
	hm->start();
}
