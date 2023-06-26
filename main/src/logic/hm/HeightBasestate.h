/*
 * HeightBasestate.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */
#pragma once

#include "HeightActions.h"
#include "HeightContextData.h"
#include "logger/logger.hpp"

enum HeightState { HEIGHT_NONE, WAIT_FOR_WS, WAIT_FOR_BELT };

class HeightBasestate {
  protected:
    HeightContextData *data;
    HeightActions *actions;

  public:
    virtual ~HeightBasestate(){};

    virtual void setData(HeightContextData *data) { this->data = data; }
    virtual void setAction(HeightActions *actions) { this->actions = actions; }

    virtual HeightState getCurrentState() { return HEIGHT_NONE; };

    virtual void entry() {}
    virtual void exit() {}

    /**
     * Height BELT was detected
     */
    virtual bool beltDetected() { return false; }

    /**
     * A new workpiece was detected at HeightSensor.
     * (height change from 0-2mm (belt) to > 2mm)
     *
     * @param height Measured height
     */
    virtual bool workpieceHeightDetected(float height) { return false; }

    /**
     * The motor was stopped
     */
    virtual bool motorStopped() { return false; }

    /**
     * The motor is running
     */
    virtual bool motorRunning() { return false; }
};
