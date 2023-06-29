/*
 * WorkpieceManager.cpp
 *
 *  Created on: 9.06.2023
 *      Author: Sulaiman
 */

#include "WorkpieceManager.h"
#include "configuration/Configuration.h"
#include "logger/logger.hpp"

WorkpieceManager::WorkpieceManager() : nextId(1) {
	ramp_one_B = false;
	ramp_two_B = false;
	auto confOrder = Configuration::getInstance().getDesiredOrder();
	for (int i = 0; i < 3; i++) {
		desiredOrder[i] = confOrder.at(i);
	}
}

WorkpieceManager::~WorkpieceManager() {}

void WorkpieceManager::rotateNextWorkpieces() {
	//    0          1          2
	// <Typ-A> -> <Typ-B> -> <Typ-C>
	// <Typ-B> -> <Typ-C> -> <Typ-A>
    WorkpieceType front = desiredOrder[0];
    desiredOrder[0] = desiredOrder[1];
    desiredOrder[1] = desiredOrder[2];
    desiredOrder[2] = front;

    printCurrentOrder();
}

void WorkpieceManager::revertNextWorkpiece() {
	//    0          1          2
	// <Typ-A> -> <Typ-B> -> <Typ-C>
	// <Typ-C> -> <Typ-A> -> <Typ-B>
    WorkpieceType front = desiredOrder[0];
    desiredOrder[0] = desiredOrder[2];
    desiredOrder[2] = desiredOrder[1];
    desiredOrder[1] = front;

    Logger::info("Workpiece order was manually resetted");
    printCurrentOrder();
}

void WorkpieceManager::printCurrentOrder() {
    std::stringstream ss;
    ss << WP_TYPE_TO_STRING(desiredOrder[0]);
    ss << " -> " << WP_TYPE_TO_STRING(desiredOrder[1]);
    ss << " -> " << WP_TYPE_TO_STRING(desiredOrder[2]);
    Logger::info("Next expected workpieces: " + ss.str());
}

WorkpieceType WorkpieceManager::getNextWorkpieceType() {
    return desiredOrder[0];
}

Workpiece *WorkpieceManager::addWorkpiece() {
    Workpiece *wp = new Workpiece();
    wp->id = nextId++;
    addToArea(AreaType::AREA_A, wp);
    return wp;
}

void WorkpieceManager::addToArea(AreaType area, Workpiece *wp) {
    switch (area) {
    case AreaType::AREA_A:
        Area_A.push(wp);
        break;
    case AreaType::AREA_B:
        Area_B.push(wp);
        break;
    case AreaType::AREA_C:
        Area_C.push(wp);
        break;
    case AreaType::AREA_D:
        Area_D.push(wp);
        break;
    }
}

void WorkpieceManager::moveFromAreaToArea(AreaType source,
                                          AreaType destination) {
    Workpiece *wp = removeFromArea(source);
    if (wp != nullptr) {
        addToArea(destination, wp);
    }
}

Workpiece *WorkpieceManager::removeFromArea(AreaType area) {
    std::queue<Workpiece *> &targetArea = getArea(area);
    if (!targetArea.empty()) {
        Workpiece *wp = targetArea.front();
        targetArea.pop();
        return wp;
    }
    return nullptr;
}

Workpiece *WorkpieceManager::getHeadOfArea(AreaType area) {
    std::queue<Workpiece *> &targetArea = getArea(area);
    if (!targetArea.empty()) {
        return targetArea.front();
    }
    return nullptr;
}

void WorkpieceManager::setHeight(AreaType area, double height) {
    Workpiece *wp = getHeadOfArea(area);
    if (wp != nullptr) {
    	if(area == AreaType::AREA_D) {
    		wp->avgHeightFBM2 = height;
    	} else {
			wp->avgHeight = height;
    	}
    }
}

void WorkpieceManager::setTypeEvent(EventType event, AreaType area) {
    WorkpieceType tmp = WorkpieceType::WS_UNKNOWN;
    if (event == EventType::HM_M_WS_F || event == EventType::HM_S_WS_F) {
        tmp = WorkpieceType::WS_F;
    } else if (event == EventType::HM_M_WS_OB || event == EventType::HM_S_WS_OB) {
        tmp = WorkpieceType::WS_OB;
    } else if (event == EventType::HM_M_WS_BOM || event == EventType::HM_S_WS_BOM) {
        tmp = WorkpieceType::WS_BOM;
    } else if (event == EventType::HM_M_WS_UNKNOWN || event == EventType::HM_S_WS_UNKNOWN) {
        tmp = WorkpieceType::WS_UNKNOWN;
    }

    Workpiece *wp = getHeadOfArea(area);
    if (wp != nullptr) {
        if (area == AreaType::AREA_D) {
            wp->S_type = tmp;
        } else {
            wp->M_type = tmp;
        }
    }
}

void WorkpieceManager::setMetal(AreaType area) {
    Workpiece *wp = getHeadOfArea(area);
    if (wp != nullptr) {
        wp->metal = true;
    }
}

void WorkpieceManager::setType(AreaType area, WorkpieceType type) {
    Workpiece *wp = getHeadOfArea(area);
    if (wp != nullptr) {
        if (area == AreaType::AREA_A || area == AreaType::AREA_B) {
            wp->M_type = type;
        } else if (area == AreaType::AREA_C || area == AreaType::AREA_D) {
            wp->S_type = type;
        }
    }
}

void WorkpieceManager::setSortOut(AreaType area, bool sortOut) {
    Workpiece *wp = getHeadOfArea(area);
    if (wp != nullptr) {
        wp->sortOut = sortOut;
    }
}

void WorkpieceManager::setFlipped(AreaType area) {
    Workpiece *wp = getHeadOfArea(area);
    if (wp != nullptr) {
        wp->flipped = true;
    }
}

void WorkpieceManager::setRamp_one(bool input){
	ramp_one_B= input;
}
void WorkpieceManager::setRamp_two(bool input){
	ramp_two_B = input;
}

bool WorkpieceManager::getRamp_one(){
	Logger::debug("Ramp one = " + std::to_string(ramp_one_B));
	return ramp_one_B;
}

bool WorkpieceManager::getRamp_two(){
	Logger::debug("Ramp two = " + std::to_string(ramp_two_B));
	return ramp_two_B;
}

bool WorkpieceManager::isFBM_MEmpty()
{
	return Area_A.empty() && Area_B.empty() && Area_C.empty();
}

bool WorkpieceManager::isFBM_SEmpty() { return Area_D.empty(); }

bool WorkpieceManager::isQueueempty(AreaType area) {
    if (area == AreaType::AREA_A) {
        return Area_A.empty();
    } else if (area == AreaType::AREA_B) {
        return Area_B.empty();
    } else if (area == AreaType::AREA_C) {
        return Area_C.empty();
    } else if (area == AreaType::AREA_D) {
        return Area_D.empty();
    }
    return false;
}

std::string WorkpieceManager::to_string_Workpiece(Workpiece *wp) {
	std::stringstream ss;
	ss << "WS at FBM1 [id=" << wp->id;
	ss << ", Type: " << WP_TYPE_TO_STRING(wp->M_type);
	ss << ", Average Height: " << std::fixed << std::setprecision(1) << wp->avgHeight << " mm]";

    return ss.str();
}

std::string WorkpieceManager::to_string_Workpiece_FBM2(Workpiece *wp) {
	std::stringstream ss;
	ss << "WS at FBM2 [id=" << wp->id;
	ss << ", Type: " << WP_TYPE_TO_STRING(wp->S_type);
	ss << ", Average Height: " << std::fixed << std::setprecision(1) << wp->avgHeightFBM2;
	ss << ", Flipped: " << (wp->flipped ? "true" : "false") << "]";

	return ss.str();
}

std::queue<Workpiece *> &WorkpieceManager::getArea(AreaType area) {
    switch (area) {
    case AreaType::AREA_A:
        return Area_A;
    case AreaType::AREA_B:
        return Area_B;
    case AreaType::AREA_C:
        return Area_C;
    case AreaType::AREA_D:
        return Area_D;
    default:
        throw std::invalid_argument("Invalid area type");
    }
}

void WorkpieceManager::reset_wpm(){
	std::queue<Workpiece*>().swap(Area_A);
	std::queue<Workpiece*>().swap(Area_B);
	std::queue<Workpiece*>().swap(Area_C);
	std::queue<Workpiece*>().swap(Area_D);
	nextId = 1;
	Logger::info("Workpieces were resetted - start sorting from the beginning");
}
