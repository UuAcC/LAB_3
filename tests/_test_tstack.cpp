#include "TStack.h"
#include <gtest/gtest.h>

TEST(TStack, can_assign_any_TStack_with_positive_lenght) {
	EXPECT_NO_THROW(TStack<int> stack(3));
	EXPECT_NO_THROW(TStack<double> stack(3));
	EXPECT_NO_THROW(TStack<TStack<int>> stack(3));
}

TEST(TStack, can_get_size) {
	TStack<int> stack(3);
	EXPECT_EQ(3, stack.get_size());
}

TEST(TStack, new_TStack_is_empty) {
	TStack<int> stack(3);
	EXPECT_TRUE(stack.isEmpty());
}
TEST(TStack, isFull_eq_true_when_full) {
	TStack<int> stack(1); stack.push(1);
	EXPECT_TRUE(stack.isFull());
}

TEST(TStack, can_get_top_of_not_empty) {
	TStack<int> stack(1);
	int n = 5431; stack.push(n);
	EXPECT_EQ(n, stack.top());
}
TEST(TStack, cant_get_top_of_empty) {
	TStack<int> stack(1); 
	EXPECT_ANY_THROW(stack.top());
}

TEST(TStack, can_push_into_not_full) {
	TStack<int> stack(3);
	int n = 5431; stack.push(n);
	int n1 = stack.top();
	EXPECT_EQ(n, n1);
}
TEST(TStack, cant_push_into_full) {
	TStack<int> stack(1); stack.push(1);
	EXPECT_ANY_THROW(stack.push(1));
}

TEST(TStack, can_pop_from_not_empty) {
	TStack<int> stack(1);
	int n = 5431; stack.push(n);
	int n1 = stack.pop();
	EXPECT_EQ(n, n1);
}
TEST(TStack, cant_pop_from_empty) {
	TStack<int> stack(1);
	EXPECT_ANY_THROW(stack.pop());
}

TEST(TStack, can_create_copied_TStack) {
	TStack<int> original(3);
	original.push(1);
	original.push(2);
	original.push(3);

	TStack<int> copy(original);

	EXPECT_EQ(original.pop(), 3);
	EXPECT_EQ(copy.pop(), 3);
	EXPECT_EQ(original.pop(), 2);
	EXPECT_EQ(copy.pop(), 2);
	EXPECT_EQ(original.pop(), 1);
	EXPECT_EQ(copy.pop(), 1);
}
TEST(TStack, can_copy_assign_TStack) {
	TStack<int> original(3);
	original.push(20);
	original.push(10);

	TStack<int>copy(1);
	copy = original;

	EXPECT_EQ(original.get_size(), copy.get_size());

	EXPECT_EQ(original.pop(), copy.pop());
	EXPECT_EQ(original.pop(), copy.pop());
}

TEST(TStack, can_create_moved_TStack) {
	TStack<int> original(3);
	original.push(1);
	original.push(2);
	original.push(3);

	TStack<int> copy(std::move(original));

	EXPECT_EQ(original.get_size(), 0);
	EXPECT_EQ(copy.get_size(), 3);

	EXPECT_EQ(copy.pop(), 3);
	EXPECT_EQ(copy.pop(), 2);
	EXPECT_EQ(copy.pop(), 1);
}
TEST(TStack, can_move_assign_TStack) {
	TStack<int> original(3);
	original.push(20);
	original.push(10);

	TStack<int>copy(1);
	copy = std::move(original);

	EXPECT_EQ(original.get_size(), 0);
	EXPECT_EQ(copy.get_size(), 3);

	EXPECT_EQ(10, copy.pop());
	EXPECT_EQ(20, copy.pop());
}

TEST(TStack, TStack_from_vector) {
	std::vector<int> vec = { 1, 2, 3, 4, 5 };
	TStack<int> s(vec);

	EXPECT_EQ(s.get_size(), 5);
	EXPECT_FALSE(s.isEmpty());

	for (int i = 4; i >= 0; i--) {
		EXPECT_EQ(s.pop(), vec[i]);
	}
}

TEST(TStack, can_clear) {
	TStack<int> s(3);
	s.push(20); s.push(10);
	EXPECT_FALSE(s.isEmpty());
	EXPECT_NO_THROW(s.pop());
	s.clear();
	EXPECT_TRUE(s.isEmpty());
	EXPECT_ANY_THROW(s.pop());
}

TEST(TStack, LIFO_behavior) {
	TStack<int> s(3);
	s.push(1);
	s.push(2);
	s.push(3);

	EXPECT_EQ(s.pop(), 3);
	EXPECT_EQ(s.pop(), 2);
	EXPECT_EQ(s.pop(), 1);
}