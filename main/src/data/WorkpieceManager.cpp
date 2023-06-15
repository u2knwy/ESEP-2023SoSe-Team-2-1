/*
 * WorkpieceManager.cpp
 *
 *  Created on: 9.06.2023
 *      Author: Sulaiman
 */

#include "WorkpieceManager.h"

WorkpieceManager::WorkpieceManager() {
	nextId = 1;
}

WorkpieceManager::~WorkpieceManager() {

}

Workpiece* WorkpieceManager::addWorkpiece() {
	Workpiece* wp;
	wp->id=nextId;
	nextId++;
	return wp;
}



void WorkpieceManager::addToArea_A(Workpiece* wp) {
	Area_A.push(wp);
}

void WorkpieceManager::addToArea_B(Workpiece* wp) {
	Area_B.push(wp);
}

void WorkpieceManager::addToArea_C(Workpiece* wp) {
	Area_C.push(wp);
}

void WorkpieceManager::addToArea_D(Workpiece* wp) {
	Area_D.push(wp);
}


void WorkpieceManager::moveFromArea_AtoArea_B(){
    addToArea_B(removeFromArea_A());
};

void WorkpieceManager::moveFromArea_BtoArea_C(){
    addToArea_C(removeFromArea_B());
};

void WorkpieceManager::moveFromArea_CtoArea_D(){
    addToArea_D(removeFromArea_C());
};

Workpiece* WorkpieceManager::removeFromArea_A() {
    Workpiece* wp = Area_A.front();
    Area_A.pop();
    return wp;
}

Workpiece*  WorkpieceManager::removeFromArea_B() {
    Workpiece* wp = Area_B.front();
    Area_B.pop();
    return wp;
}

Workpiece*  WorkpieceManager::removeFromArea_C() {
    Workpiece* wp = Area_C.front();
    Area_C.pop();
    return wp;
}

Workpiece* WorkpieceManager::getter_head_Area_A(){
	Workpiece* wp = Area_A.front();
	return wp;
}

Workpiece* WorkpieceManager::getter_head_Area_B(){
	Workpiece* wp = Area_B.front();
	return wp;
}

Workpiece* WorkpieceManager::getter_head_Area_C(){
	Workpiece* wp = Area_C.front();
	return wp;
}

Workpiece* WorkpieceManager::getter_head_Area_D(){
	Workpiece* wp = Area_D.front();
	return wp;
}

Workpiece*  WorkpieceManager::removeFromArea_D() {
    Workpiece* wp = Area_D.front();
    Area_D.pop();
    return wp;
}


void WorkpieceManager::setHeight_M(int height){
	if (!Area_A.empty()) {
		Workpiece* currentWorkpiece = Area_A.front();
		currentWorkpiece->avgHeight = height;
	}
}
void WorkpieceManager::setHeight_S(int height){
	if (!Area_D.empty()) {
		float tmp= height/10;
		Workpiece* currentWorkpiece = Area_D.front();
		currentWorkpiece->avgHeight = tmp;
	}
}

void WorkpieceManager::setMetal_M(){
	if (!Area_B.empty()) {
		Workpiece* currentWorkpiece = Area_B.front();
		currentWorkpiece->metal = true;
	}
}

void WorkpieceManager::setMetal_S(){
	if (!Area_D.empty()) {
		Workpiece* currentWorkpiece = Area_D.front();
		currentWorkpiece->metal = true;
	}
}

void WorkpieceManager::setType_M(WorkpieceType type){
	if (!Area_B.empty()) {
		Workpiece* currentWorkpiece = Area_B.front();
		currentWorkpiece->WP_M_type = type;
	}
}

void WorkpieceManager::setType_S(WorkpieceType type){
	if (!Area_D.empty()) {
		Workpiece* currentWorkpiece = Area_D.front();
		currentWorkpiece->WP_S_type = type;
	}
}

void WorkpieceManager::setSortOut_M(bool input){
	if (!Area_B.empty()) {
		Workpiece* currentWorkpiece = Area_B.front();
		currentWorkpiece->sortOut = input;
	}
}

void WorkpieceManager::setSortOut_S(bool input){
	if (!Area_D.empty()) {
		Workpiece* currentWorkpiece = Area_D.front();
		currentWorkpiece->sortOut = input;
	}
}

bool WorkpieceManager::getSortOut_M(){
	if (!Area_B.empty()) {
		Workpiece* currentWorkpiece = Area_B.front();
		return currentWorkpiece->sortOut;
	}
}

bool WorkpieceManager::getSortOut_S(){
	if (!Area_D.empty()) {
		Workpiece* currentWorkpiece = Area_D.front();
		return currentWorkpiece->sortOut;
	}
}

void WorkpieceManager::setflipped(){
	if (!Area_D.empty()) {
		Workpiece* currentWorkpiece = Area_D.front();
		currentWorkpiece->flipped=true;
	}
}

void WorkpieceManager::print_WP(){
	if (!Area_D.empty()) {
		Workpiece* currentWorkpiece = Area_D.front();
		std::cout << "WP: id = "<< currentWorkpiece->id <<std::endl;
		std::cout << "    metal = "<< currentWorkpiece->metal <<std::endl;
		std::cout << "    Height = "<< currentWorkpiece->avgHeight <<std::endl;
		std::cout << "    flipped = "<< currentWorkpiece->flipped <<std::endl;
		std::cout << "    Type = "<< currentWorkpiece->WP_S_type <<std::endl;
	}
}

bool WorkpieceManager::WP_ON_FBM_M() {
	return Area_A.empty() && Area_B.empty() &&Area_C.empty();
}

bool WorkpieceManager::fbm_S_Occupied() {
	return Area_D.empty();
}


/*
 * test code
 */
void  WorkpieceManager::testWorkpieceManager(){
    Workpiece* wp1;
    Workpiece* wp2;
    Workpiece* wp3;
    Workpiece* wp4;
    Workpiece* wp5;
    Workpiece* wp6;
    Workpiece* wp7;
    Workpiece* wp8;

    wp1->id =1;
    wp2->id =2;
    wp3->id =3;
    wp4->id =4;
    wp5->id =5;
    wp6->id =6;
    wp7->id =7;
    wp8->id =8;

    addToArea_A(wp1);
    addToArea_A(wp2);
    addToArea_A(wp3);
    addToArea_A(wp4);
    addToArea_A(wp5);
    addToArea_A(wp6);
    addToArea_A(wp7);

    moveFromArea_AtoArea_B();
    moveFromArea_BtoArea_C();
    //removeFromArea_D();
}




