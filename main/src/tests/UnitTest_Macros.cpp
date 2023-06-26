/*
 * UnitTest_Macros.cpp
 *
 *  Created on: 26.06.2023
 *      Author: Maik
 */

#include <gtest/gtest.h>

#include "events/IEventHandler.h"
#include "events/EventSender.h"

TEST(EventDataTest, DataAndAdditionalDataNA) {
	int data = 234;
	int addData = -1;
	int eventData = (addData << 16) | data;
	int decodedData = eventData & 0xFFFF;
	int decodedAddData = (eventData >> 16);

	EXPECT_EQ(234, decodedData);
	EXPECT_EQ(-1, decodedAddData);

	Event ev;
	EXPECT_EQ(-1, ev.data);
	EXPECT_EQ(-1, ev.additional_data);
}

TEST(EventDataTest, EventWithDataAndNoAdditionalData) {
	Event ev{(EventType) 12, 24};
	EXPECT_EQ(12, ev.type);
	EXPECT_EQ(24, ev.data);
	EXPECT_EQ(-1, ev.additional_data);
}

TEST(EventDataTest, EventWithAdditionalData) {
	Event ev{(EventType) 12, 24, 36};
	EXPECT_EQ(12, ev.type);
	EXPECT_EQ(24, ev.data);
	EXPECT_EQ(36, ev.additional_data);
	EXPECT_EQ(2359320, EVENT_DATA(ev));
	EXPECT_EQ(24, DATA_FROM_EVENT(2359320));
	EXPECT_EQ(36, ADD_DATA_FROM_EVENT(2359320));
}

TEST(EventDataTest, EventDataCombined) {
	Event ev{(EventType) 12, 20, 16};
	EXPECT_EQ(12, ev.type);
	EXPECT_EQ(20, ev.data);
	EXPECT_EQ(16, ev.additional_data);
	EXPECT_EQ(1048596, EVENT_DATA(ev));
	EXPECT_EQ(20, DATA_FROM_EVENT(1048596));
	EXPECT_EQ(16, ADD_DATA_FROM_EVENT(1048596));
}
