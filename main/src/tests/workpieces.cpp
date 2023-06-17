/*
 * workpieces.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */
#include <gtest/gtest.h>
#include "data/WorkpieceManager.h"
#include "data/Workpiece.h"

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
	EXPECT_TRUE(mngr->isFBM_SOccupied());
	Workpiece* wp1 = mngr->addWorkpiece();
	EXPECT_EQ(1, wp1->id);
	Workpiece* wp2 = mngr->addWorkpiece();
	EXPECT_EQ(2, wp2->id);
	Workpiece* wp1a = mngr->getHeadOfArea(AreaType::AREA_A);
	EXPECT_EQ(wp1->id, wp1a->id);
}
