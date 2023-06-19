/*
 * MainContextData.h
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */
#pragma once
#include "data/Workpiece.h"
#include "data/WorkpieceManager.h"

class MainContextData {
  public:
    MainContextData();
    virtual ~MainContextData();
    WorkpieceManager *wpManager;
    void setRampFBM1Blocked(bool blocked);
    void setRampFBM2Blocked(bool blocked);
    bool isRampFBM1Blocked();
    bool isRampFBM2Blocked();

  private:
    bool rampFBM1Blocked;
    bool rampFBM2Blocked;
};
