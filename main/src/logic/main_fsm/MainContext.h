/*
 * MainContext.h
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */
#pragma once

#include "MainActions.h"
#include "MainBasestate.h"
#include "MainContextData.h"
#include "events/EventManager.h"

#include <memory>

class MainContext {
public:
	MainContext(std::shared_ptr<EventManager> mngr);
	virtual ~MainContext();

	MainState getCurrentState();

	void master_LBA_Blocked();
	void master_LBA_Unblocked();
	void master_LBW_Blocked();
	void master_LBW_Unblocked();
	void master_LBE_Blocked();
	void master_LBE_Unblocked();
	void master_LBR_Blocked();
	void master_LBR_Unblocked();

	void slave_LBA_Blocked();
	void slave_LBA_Unblocked();
	void slave_LBW_Blocked();
	void slave_LBW_Unblocked();
	void slave_LBE_Blocked();
	void slave_LBE_Unblocked();
	void slave_LBR_Blocked();
	void slave_LBR_Unblocked();

	// Buttons
	void master_btnStart_PressedShort();
	void master_btnStart_PressedLong();
	void master_btnStop_Pressed();
	void master_btnReset_Pressed();

	void slave_btnStart_PressedShort();
	void slave_btnStart_PressedLong();
	void slave_btnStop_Pressed();
	void slave_btnReset_Pressed();

	// EStop
	void master_EStop_Pressed();
	void master_EStop_Released();

	void slave_EStop_Pressed();
	void slave_EStop_Released();

private:
	std::unique_ptr<MainActions> actions;
	MainBasestate* state;
	std::unique_ptr<MainContextData> data;
	std::shared_ptr<EventManager> eventManager;
	void subscribeToEvents();
};
