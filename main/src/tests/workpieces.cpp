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

	EXPECT_FALSE(mngr->fbm_S_Occupied());
	//EXPECT_FALSE(mngr->removeWpOnFBM2());
	//EXPECT_FALSE(mngr->wpTransferredToFBM2());

	//EXPECT_EQ(0, mngr->nWorkpiecesOnFBM1());
	//Workpiece wp1 = mngr->addWorkpiece();
	//EXPECT_EQ(1, wp1->getId());
	//EXPECT_EQ(1, mngr->nWorkpiecesOnFBM1());

	//Workpiece wp2 = mngr->addWorkpiece();
	//EXPECT_EQ(2, wp2->getId());
	//EXPECT_EQ(2, mngr->nWorkpiecesOnFBM1());

}
