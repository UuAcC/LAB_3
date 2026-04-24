#include <gtest/gtest.h>
#include "Pascal_MinusMinus_Expression.h"

static void run_PMME(string code) {
	cout << code << "\n\n";
	PMM_EXPR _(code);
	_.execute();
	cout << "\n";
}

TEST(PMM_EXPR, Basic_Assignment_and_Calculations) {
	string code = "a = 34; b = a + a; a = b";
	EXPECT_NO_THROW(run_PMME(code));
}

TEST(PMM_EXPR, Zero_Division) {
	string code = "a = 34; b = a / 0";
	EXPECT_NO_THROW(run_PMME(code));
}

TEST(PMM_EXPR, Error_Variable_Not_Inited) {
	string code = "a = b; b = a + 1";
	EXPECT_NO_THROW(run_PMME(code));
}

TEST(PMM_EXPR, If_Else_Workability) {
	string code = "a = 4; i = 0; if (a > i) { i = 7 }; if (a > i) { a = 34 } else { a = 49 }";
	EXPECT_NO_THROW(run_PMME(code));
}

TEST(PMM_EXPR, While_Workability) {
	string code = "i = 0; while (i < 10) { i = i + 1 }";
	EXPECT_NO_THROW(run_PMME(code));
}

TEST(PMM_EXPR, Difficult_Calculations) {
	string code = "a = ((325 - 45.231) * 78 - 23434 * 1.00);"; 
	EXPECT_NO_THROW(run_PMME(code));
}