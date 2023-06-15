/*
 * WorkpieceManager.h
 *
 *  Created on: 9.06.2023
 *      Author: Sulaiman
 */

#pragma once

#include "Workpiece.h"
#include <iostream>

#include <queue>

class WorkpieceManager {
private:
	std::queue<Workpiece*> Area_A;
	std::queue<Workpiece*> Area_B;
	std::queue<Workpiece*> Area_C;
	std::queue<Workpiece*> Area_D;
	int nextId;
public:
	WorkpieceManager();
	virtual ~WorkpieceManager();

	/**
	 * creats a new Workpiece with default values.
	 *
	 * @return Workpiece* Reference to the created workpiece
	 */
	Workpiece* addWorkpiece();

	/**
	 * Adds Workpeice to the Lists(fifo principle) (Area_A,Area_B,Area_C,Area_D);
	 */

    void addToArea_A(Workpiece* wp);

    void addToArea_B(Workpiece* wp);

    void addToArea_C(Workpiece* wp);

    void addToArea_D(Workpiece* wp);

    /**
	 * Removes Workpeice from one area and adds to another area;
	 */

	void moveFromArea_AtoArea_B();

	void moveFromArea_BtoArea_C();

	void moveFromArea_CtoArea_D();

    /**
	 * Removes Workpiece from the Lists(fifo principle) (Area_A,Area_B,Area_C,Area_D);
	 */

    Workpiece* removeFromArea_A();

    Workpiece* removeFromArea_B();

    Workpiece* removeFromArea_C();

    Workpiece* removeFromArea_D();

    /**
	 * getter for head of (Area_A,Area_B,Area_C,Area_D);
	 */
    Workpiece* getter_head_Area_A();

	Workpiece* getter_head_Area_B();

	Workpiece* getter_head_Area_C();

	Workpiece* getter_head_Area_D();

    /**
	 * setter for the Workpieces that are in the Queue's ;
	 */
    void setHeight_M(int height);

    void setHeight_S(int height);

    void setMetal_M();

    void setMetal_S();

    void setType_M(WorkpieceType type);

    void setType_S(WorkpieceType type);

    void setSortOut_M(bool input);

    void setSortOut_S(bool input);

    bool getSortOut_M();

    bool getSortOut_S();

    void setflipped();

    /**
	 * Prints the information of Workpiece that comes at the end of FBM_S;
	 */

    void print_WP();

    /**
	 * @return boolean if there is/are Workpiece/s on FBM1
	 */
	bool WP_ON_FBM_M();

	/**
	 * Checks the "occupied" status of FBM2
	 *
	 * @return true if FBM2 is occupied (one workpiece on belt(
	 */
	bool fbm_S_Occupied();

    /**
	 * shows the ammount of elements in each list;
	 */

	 void showAllLists(){
        std::cout << "Area_A= ";
        while (!Area_A.empty()) {
            std::cout << Area_A.front()->id << ", ";;
            Area_A.pop();
        }
        std::cout << "\nArea_B= " ;
        while (!Area_B.empty()) {
            std::cout << Area_B.front()->id << ", ";;
            Area_B.pop();
        }
        std::cout << "\nArea_C= " ;
        while (!Area_C.empty()) {
            std::cout << Area_C.front()->id << ", ";;
            Area_C.pop();
        }
        std::cout << "\nArea_d= ";
        while (!Area_D.empty()) {
            std::cout << Area_D.front()->id << ", ";;
            Area_D.pop();
        }
        std::cout << std::endl;

     }


    /**
     * tmp method for test purposes
     */
    void testWorkpieceManager();


};
