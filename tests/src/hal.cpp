/*
 * hal.cpp
 *
 *  Created on: 04.04.2023
 *      Author: Maik
 */
#include <gtest/gtest.h>
#include "hal/hal.h"

class HAL_Test : public ::testing::Test {
protected:
  HAL *hal;

  /**
   * OPTIONAL: Prepare objects before each test
   */
  void SetUp() override {
	  hal = new HAL();
  }

  /**
   * OPTIONAL: Release any resources allocated in SetUp() method
   */
  void TearDown() override {
	  delete hal;
  }
};

TEST_F(HAL_Test, GreenLamp) {
	// Example: now we can call methods on the object created in SetUp method
	hal->GreenLampOn();
	EXPECT_EQ(0, 1);
}
