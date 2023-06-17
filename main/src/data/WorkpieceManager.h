/*
 * WorkpieceManager.h
 *
 *  Created on: 9.06.2023
 *      Author: Sulaiman
 */

#ifndef WORKPIECEMANAGER_H_
#define WORKPIECEMANAGER_H_

#include <iostream>
#include <queue>
#include <string>
#include "Workpiece.h"
#include "events/events.h"

enum class AreaType {
    AREA_A,
    AREA_B,
    AREA_C,
    AREA_D
};

class WorkpieceManager {
public:
    WorkpieceManager();
    ~WorkpieceManager();

    void rotateNextWorkpieces();
    WorkpieceType getNextWorkpieceType();
    Workpiece* addWorkpiece();

    void addToArea(AreaType area, Workpiece* wp);
    void moveFromAreaToArea(AreaType sourceArea, AreaType destinationArea);
    Workpiece* removeFromArea(AreaType area);
    Workpiece* getHeadOfArea(AreaType area);

    void setHeight(AreaType area, double height);
    void setMetal(AreaType area);
    void setType(AreaType area, WorkpieceType type);
    void setTypeEvent(EventType event,AreaType area);
    void setSortOut(AreaType area, bool sortOut);
    void setFlipped(AreaType area);
    bool getSortOut(AreaType area);
    bool getFlipped(AreaType area);


    bool isFBM_MEmpty();
    bool isFBM_SOccupied();
    bool isQueueempty(AreaType area);

    void printWorkpiece(Workpiece* wp);

    void testWorkpieceManager();



private:
    int nextId;
    WorkpieceType desiredOrder[3];
    std::queue<Workpiece*> Area_A;
    std::queue<Workpiece*> Area_B;
    std::queue<Workpiece*> Area_C;
    std::queue<Workpiece*> Area_D;

    Workpiece* getQueue(AreaType area);
    std::queue<Workpiece*>& getArea(AreaType area);
};

#endif /* WORKPIECEMANAGER_H_ */

