/*
 * WorkpieceManager.h
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#pragma once

#include "Workpiece.h"

#include <vector>
#include <list>

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
	 * Finds the workpiece at the HeightSensor and sets its result.
	 *
	 * @param result Result of the height measurement
	 * @return true if result was set, false if no workpiece is at FBM1 HeightSensor
	 */
	bool heightResultFBM1Received(WorkpieceType result, float avgHeight);

	/**
	 * Finds the workpiece at the metal sensor and sets its result.
	 *
	 * @param isMetal Sets if metal was detected.
	 * @return true if result was set, false if no workpiece is at MetalSensor
	 */
	bool metalDetectedFBM1(bool isMetal);

	/**
	 * Check if workpiece which will arrive at switch must be sorted out.
	 *
	 * @return true if the workpiece has to be sorted out.
	 */
	bool nextWorkpieceAtFBM1Valid();

	/**
	 * Updates the state of the workpiece which is currently at the switch of FBM1.
	 *
	 * @return true if workpiece was updated
	 */
	bool nextWorkpieceAtFBM1SetSortOut();

	/**
	 * A workpiece at FBM1 was sorted out.
	 * This method will remove it from the internal list and the number of workpieces on FBM1 will be decremented.
	 *
	 * @return true if workpiece was removed from internal list
	 */
	bool workpieceAtFBM1SortedOut();

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
	bool workpieceAtFBM2SortedOut();

	/**
	 * Transfer the oldest workpiece on FBM1 to FBM2.
	 *
	 * @return true if the transfer was successful, false if there are no workpieces on FBM1.
	 */
	bool transferWorkpieceToFBM2();
private:
	int nextId;
	std::vector<Workpiece*> workpiecesOnFBM1;
	Workpiece* workpieceOnFBM2;
	std::list<WorkpieceType> desiredOrder;

	/**
	 * Rotates the "ring-buffer" of next-expected Workpiece types.
	 * (Remove next-expected type and insert at the back)
	 */
	void rotateWorkpieceOrder();
};
