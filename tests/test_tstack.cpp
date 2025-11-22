#include "TStack.h"
#include <gtest/gtest.h>

TEST(TStack, Can_assign_int_TStack_with_positive_lenght) {
	EXPECT_NO_THROW(TStack<int> stack(3));
}