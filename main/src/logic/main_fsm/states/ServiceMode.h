/*
 * Running.h
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */
#pragma once

#include "../MainBasestate.h"

class ServiceMode : public MainBasestate {
    void entry() override;
    void exit() override;

    MainState getCurrentState() override;

    void initSubStateServiceMode() override;

    bool master_LBA_Blocked() override;
    bool master_LBA_Unblocked() override;
    bool master_LBW_Blocked() override;
    bool master_LBW_Unblocked() override;
    bool master_LBE_Blocked() override;
    bool master_LBE_Unblocked() override;
    bool master_LBR_Blocked() override;
    bool master_LBR_Unblocked() override;

    bool slave_LBA_Blocked() override;
    bool slave_LBA_Unblocked() override;
    bool slave_LBW_Blocked() override;
    bool slave_LBW_Unblocked() override;
    bool slave_LBE_Blocked() override;
    bool slave_LBE_Unblocked() override;
    bool slave_LBR_Blocked() override;
    bool slave_LBR_Unblocked() override;

    bool master_btnStart_PressedShort() override;
    bool master_btnStop_Pressed() override;
    bool master_btnReset_Pressed() override;

    bool slave_btnStart_PressedShort() override;
    bool slave_btnStop_Pressed() override;
    bool slave_btnReset_Pressed() override;

    bool master_EStop_Pressed() override;
    bool slave_EStop_Pressed() override;
};
