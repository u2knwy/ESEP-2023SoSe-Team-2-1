/*
 * SubEStopOnePressed.h
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */
#pragma once

#include "../MainBasestate.h"

class SubServiceModeSelftestActuators : public MainBasestate {
    void entry() override;
    void exit() override;

    bool master_btnStart_PressedShort() override;
    bool master_btnReset_Pressed() override;

    bool slave_btnStart_PressedShort() override;
    bool slave_btnReset_Pressed() override;
};
