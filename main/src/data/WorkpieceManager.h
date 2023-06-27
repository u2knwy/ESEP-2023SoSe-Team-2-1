/*
 * WorkpieceManager.h
 *
 *  Created on: 9.06.2023
 *      Author: Sulaiman
 */

#ifndef WORKPIECEMANAGER_H_
#define WORKPIECEMANAGER_H_

#include "Workpiece.h"
#include "events/events.h"
#include <iostream>
#include <queue>
#include <string>


enum class AreaType { AREA_A, AREA_B, AREA_C, AREA_D };

class WorkpieceManager {
  public:
    WorkpieceManager();
    ~WorkpieceManager();

    void rotateNextWorkpieces();

    void revertNextWorkpiece();

    /**
     * Prints the current order in which workpieces should arrive at the end of
     * FBM2.
     */
    void printCurrentOrder();

    WorkpieceType getNextWorkpieceType();
    Workpiece *addWorkpiece();

    void addToArea(AreaType area, Workpiece *wp);
    void moveFromAreaToArea(AreaType sourceArea, AreaType destinationArea);
    Workpiece *removeFromArea(AreaType area);
    Workpiece *getHeadOfArea(AreaType area);

    void setHeight(AreaType area, double height);
    void setMetal(AreaType area);
    void setType(AreaType area, WorkpieceType type);
    void setTypeEvent(EventType event, AreaType area);
    void setSortOut(AreaType area, bool sortOut);
    void setFlipped(AreaType area);
    void setRamp_one(bool input);
    void setRamp_two(bool input);
    bool getSortOut(AreaType area);
    bool getFlipped(AreaType area);
    bool getRamp_one();
    bool getRamp_two();

    bool isFBM_MEmpty();
    bool isFBM_SEmpty();
    bool isQueueempty(AreaType area);

    void reset_wpm();
    std::string to_string_Workpiece(Workpiece *wp);

    void testWorkpieceManager();

  private:
    int nextId;
    WorkpieceType desiredOrder[3];
    std::queue<Workpiece*> Area_A;
    std::queue<Workpiece*> Area_B;
    std::queue<Workpiece*> Area_C;
    std::queue<Workpiece*> Area_D;
    bool ramp_one_B;
    bool ramp_two_B;

    Workpiece *getQueue(AreaType area);
    std::queue<Workpiece *> &getArea(AreaType area);
};

#endif /* WORKPIECEMANAGER_H_ */
