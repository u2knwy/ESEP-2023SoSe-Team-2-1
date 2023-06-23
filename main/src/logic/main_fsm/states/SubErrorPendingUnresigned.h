/*
 * SubErrorPendingUnresigned.h
 *
 *  Created on: 14.06.2023
 *      Author: Maik
 */
#pragma once

#include "../MainBasestate.h"

class SubErrorPendingUnresigned : public MainBasestate {
    void entry() override;
    void exit() override;

    bool selfSolvableErrorOccurred() override;
    bool nonSelfSolvableErrorOccurred() override;

    bool errorSelfSolved() override;
    bool master_btnReset_Pressed() override;
    bool slave_btnReset_Pressed() override;

  private:
    bool selfSolving{false};
    bool manualSolving{false};
};
