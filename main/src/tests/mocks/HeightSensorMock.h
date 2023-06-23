/*
 * HeightSensorMock.h
 *
 *  Created on: 05.06.2023
 *      Author: Maik
 */

#ifndef SRC_TESTS_HEIGHTSENSORMOCK_H_
#define SRC_TESTS_HEIGHTSENSORMOCK_H_

#include "hal/IHeightSensor.h"

class HeightSensorMock : public IHeightSensor {
  public:
    HeightSensorMock();
    void registerOnNewValueCallback(HeightCallback callback) override;
    void unregisterOnNewValueCallback() override;
    void start() override;
    void stop() override;
    float getAverageHeight() override;
    float getMaxHeight() override;
    float getMedianHeight() override;
    int getLastRawValue() override;
};

#endif /* SRC_TESTS_HEIGHTSENSORMOCK_H_ */
