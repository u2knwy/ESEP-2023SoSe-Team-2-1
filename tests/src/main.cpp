#include <iostream>
#include <gtest/gtest.h>

int foo(int value) {
	return 0;
}

TEST(main, test_foo) {
	EXPECT_EQ(0, foo(21));
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	auto result = RUN_ALL_TESTS();

	return 0;
}
