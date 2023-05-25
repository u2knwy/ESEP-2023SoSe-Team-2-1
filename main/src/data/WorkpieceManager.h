/*
 * WorkpieceManager.h
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#pragma once

#include "Workpiece.h"

#include <vector>

class WorkpieceManager {
public:
	WorkpieceManager();
	virtual ~WorkpieceManager();

	/**
	 * Add a new Workpiece to the start of FBM1.
	 *
	 * @return Workpiece* Reference to the created workpiece
	 */
	Workpiece* addWorkpiece();

	/**
	 * Get the current number of workpieces of FBM1
	 *
	 * @return number of workpieces on FBM1
	 */
	int nWorkpiecesOnFBM1();

	/**
	 * Checks the "occupied" status of FBM2
	 *
	 * @return true if FBM2 is occupied (one workpiece on belt(
	 */
	bool isFBM2Occupied();

	/**
	 * Remove the current workpiece from FBM2 (was sorted out or taken at the end)
	 *
	 * @return true if workpiece was removed, false if no workpiece is currently on FBM2
	 */
	bool removeWpOnFBM2();

	/**
	 * Transfer the oldest workpiece on FBM1 to FBM2.
	 *
	 * @return true if the transfer was successful, false if there are no workpieces on FBM1.
	 */
	bool wpTransferredToFBM2();
private:
	int nextId;
	std::vector<Workpiece*> workpiecesOnFBM1;
	Workpiece* workpieceOnFBM2;
};
