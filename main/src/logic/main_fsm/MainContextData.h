/*
 * MainContextData.h
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */
#pragma once
#include "data/Workpiece.h"
#include "data/WorkpieceManager.h"

struct SelftestSensorsResult {
    bool master_lbStartOk{false};
    bool master_lbSwitchOk{false};
    bool master_lbRampOk{false};
    bool master_lbEndOk{false};
    bool slave_lbStartOk{false};
    bool slave_lbSwitchOk{false};
    bool slave_lbRampOk{false};
    bool slave_lbEndOk{false};
};

class MainContextData {
  public:
    MainContextData();
    virtual ~MainContextData();
    WorkpieceManager *wpManager;
    void setRampFBM1Blocked(bool blocked);
    void setRampFBM2Blocked(bool blocked);
    bool isRampFBM1Blocked();
    bool isRampFBM2Blocked();

    void resetSelftestSensorsResult();
    bool getSelftestSensorsResult();

    SelftestSensorsResult ssResult;
    bool master_pusherMounted;
    bool slave_pusherMounted;

  private:
    bool rampFBM1Blocked;
    bool rampFBM2Blocked;
};
