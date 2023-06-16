/*
 * Error.h
 *
 *  Created on: 14.06.2023
 *      Author: Maik
 */
#pragma once

#include "../MainBasestate.h"

class Error: public MainBasestate {
	void entry() override;
	void exit() override;
	void initSubStateError() override;
	MainState getCurrentState() override;

	bool selfSolvableErrorOccurred() override;
	bool nonSelfSolvableErrorOccurred() override;

	bool errorSelfSolved() override;
	bool master_btnReset_Pressed() override;
	bool slave_btnReset_Pressed() override;
	bool master_btnStart_PressedShort() override;
	bool slave_btnStart_PressedShort() override;
private:
	bool selfSolving;
	bool manualSolving;
};
