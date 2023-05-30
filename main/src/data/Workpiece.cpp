/*
 * Workpiece.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "Workpiece.h"

Workpiece::Workpiece(int id) {
	this->id = id;
	this->status = WorkpieceStatus::FBM1_START_HM;
}

int Workpiece::getId() {
	return this->id;
}

void Workpiece::setType(WorkpieceType type) {
	this->type = type;
}

void Workpiece::setAvgHeightFBM1(float height) {
	this->avgHeightFBM1 = height;
}

void Workpiece::setMetalFBM1(bool metal) {
	this->metalFBM1 = metal;
}

void Workpiece::setMaxHeightFBM2(float height) {
	this->maxHeightFBM2 = height;
}

void Workpiece::setMetalFBM2(bool metal) {
	this->metalFBM1 = metal;
}

void Workpiece::setFlipped(bool flipped) {
	this->flipped = flipped;
}

WorkpieceType Workpiece::getType() {
	return type;
}

WorkpieceStatus Workpiece::getStatus() {
	return this->status;
}

void Workpiece::setStatus(WorkpieceStatus newStatus) {
	this->status = newStatus;
}
