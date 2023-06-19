/*
 * workpieces.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */
#include <gtest/gtest.h>
#include "data/WorkpieceManager.h"
#include "data/Workpiece.h"
#include "data/workpiecetype_enum.h"

class Workpieces_Test : public ::testing::Test {
protected:
  WorkpieceManager* mngr;

  /**
   * OPTIONAL: Prepare objects before each test
   */
  void SetUp() override {
	  mngr = new WorkpieceManager();
  }

  /**
   * OPTIONAL: Release any resources allocated in SetUp() method
   */
  void TearDown() override {
	  delete mngr;
  }
};

TEST_F(Workpieces_Test, InitiallyNoWorkpiecesAddNewAndIncrementId) {
	EXPECT_TRUE(mngr->isFBM_MEmpty());
	EXPECT_TRUE(mngr->isFBM_SEmpty());

	Workpiece* wp1 = mngr->addWorkpiece();                                       		//wp1 hinzugefügt
	EXPECT_EQ(1, wp1->id);
	Workpiece* wp2 = mngr->addWorkpiece();												//wp2 hinzugefügt
	EXPECT_EQ(2, wp2->id);
	Workpiece* wp1a = mngr->getHeadOfArea(AreaType::AREA_A);
	EXPECT_EQ(wp1->id, wp1a->id);

	mngr->moveFromAreaToArea(AreaType::AREA_A, AreaType::AREA_B);						//transfer wp1 von Area_A nach Area_B
	Workpiece* wp1b = mngr->getHeadOfArea(AreaType::AREA_B);
	EXPECT_EQ(wp1->id, wp1b->id);

	Workpiece* wp3 = mngr->addWorkpiece();												//wp3 hinzugefügt
	EXPECT_EQ(3, wp3->id);

	Workpiece* wp2a = mngr->getHeadOfArea(AreaType::AREA_A);
	EXPECT_EQ(wp2->id, wp2a->id);

	mngr->setMetal(AreaType::AREA_B);
	mngr->setHeight(AreaType::AREA_B, 23);
	mngr->setType(AreaType::AREA_B, WorkpieceType::WS_F);
	mngr->setSortOut(AreaType::AREA_B, true);

	EXPECT_EQ(wp1b->metal, true);
	EXPECT_EQ(wp1b->avgHeight, 23);
	EXPECT_EQ(wp1b->M_type,  WorkpieceType::WS_F);
	EXPECT_EQ(wp1b->sortOut, true);

	mngr->moveFromAreaToArea(AreaType::AREA_B, AreaType::AREA_C);
	EXPECT_EQ(mngr->isQueueempty(AreaType::AREA_A), false);
	EXPECT_EQ(mngr->isQueueempty(AreaType::AREA_B), true);
	EXPECT_EQ(mngr->isQueueempty(AreaType::AREA_C), false);

	Workpiece* wp1c = mngr->getHeadOfArea(AreaType::AREA_C);
	EXPECT_EQ(wp1c->metal, true);
	EXPECT_EQ(wp1c->avgHeight, 23);
	EXPECT_EQ(wp1c->M_type,  WorkpieceType::WS_F);
	EXPECT_EQ(wp1c->sortOut, true);
	EXPECT_EQ(wp1c->flipped, false);

	mngr->moveFromAreaToArea(AreaType::AREA_A, AreaType::AREA_B);
	Workpiece* wp2b = mngr->getHeadOfArea(AreaType::AREA_B);
	EXPECT_EQ(wp2b->metal, false);
	EXPECT_EQ(wp2b->avgHeight, 0);
	EXPECT_EQ(wp2b->M_type,  WorkpieceType::WS_UNKNOWN);
	EXPECT_EQ(wp2b->sortOut, false);
	EXPECT_EQ(wp2b->flipped, false);


	EXPECT_EQ(mngr->isQueueempty(AreaType::AREA_A), false);
	EXPECT_EQ(mngr->isQueueempty(AreaType::AREA_B), false);
	EXPECT_EQ(mngr->isQueueempty(AreaType::AREA_C), false);

	mngr->moveFromAreaToArea(AreaType::AREA_C, AreaType::AREA_D);
	Workpiece* wp1d = mngr->getHeadOfArea(AreaType::AREA_D);
	EXPECT_EQ(wp1d->metal, true);
	EXPECT_EQ(wp1d->avgHeight, 23);
	EXPECT_EQ(wp1d->M_type,  WorkpieceType::WS_F);
	EXPECT_EQ(wp1d->S_type,  WorkpieceType::WS_UNKNOWN);
	EXPECT_EQ(wp1d->sortOut, true);
	EXPECT_EQ(wp1d->flipped, false);

	EXPECT_EQ(mngr->isQueueempty(AreaType::AREA_A), false);
	EXPECT_EQ(mngr->isQueueempty(AreaType::AREA_B), false);
	EXPECT_EQ(mngr->isQueueempty(AreaType::AREA_C), true);
	EXPECT_EQ(mngr->isQueueempty(AreaType::AREA_D), false);

	mngr->setType(AreaType::AREA_D, WorkpieceType::WS_F);
	EXPECT_EQ(wp1d->S_type,  WorkpieceType::WS_F);

	EXPECT_EQ(mngr->isFBM_MEmpty(),  false);
	EXPECT_EQ(mngr->isFBM_SEmpty(),  false);












}
