#include <iostream>
#include <gtest/gtest.h>

int test(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	auto result = RUN_ALL_TESTS();

	return 0;
}
