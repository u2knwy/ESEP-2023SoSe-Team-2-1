/*
 * SubServiceModeSelftestSensors.h
 *
 *  Created on: 06.06.2023
 *      Author: Maik
 */
#pragma once

#include "../MainBasestate.h"

class SubServiceModeSelftestSensors : public MainBasestate {
    void entry() override;
    void exit() override;

    bool master_btnStart_PressedShort() override;
    bool master_btnReset_Pressed() override;

    bool slave_btnStart_PressedShort() override;
    bool slave_btnReset_Pressed() override;

    bool master_LBA_Blocked() override;
    bool master_LBW_Blocked() override;
    bool master_LBE_Blocked() override;
    bool slave_LBA_Blocked() override;
    bool slave_LBE_Blocked() override;
    bool slave_LBW_Blocked() override;
    bool master_LBR_Blocked() override;
    bool slave_LBR_Blocked() override;
};
