#include "TQueue.h"
#include <gtest/gtest.h>

TEST(TQueue, can_create_int_TQueue_with_positive_length) {
    EXPECT_NO_THROW(TQueue<int> queue(3));
}

TEST(TQueue, can_create_double_TQueue_with_positive_length) {
    EXPECT_NO_THROW(TQueue<double> queue(3));
}

TEST(TQueue, can_get_size) {
    TQueue<int> queue(3);
    EXPECT_EQ(3, queue.get_size());
}

TEST(TQueue, new_TQueue_is_empty) {
    TQueue<int> queue(3);
    EXPECT_TRUE(queue.isEmpty());
}

TEST(TQueue, isFull_eq_true_when_full) {
    TQueue<int> queue(1);
    queue.push(1);
    EXPECT_TRUE(queue.ifFull());
}

TEST(TQueue, can_get_top_of_not_empty) {
    TQueue<int> queue(1);
    int n = 5431;
    queue.push(n);
    EXPECT_EQ(n, queue.top());
}

TEST(TQueue, cant_get_top_of_empty) {
    TQueue<int> queue(1);
    EXPECT_ANY_THROW(queue.top());
}

TEST(TQueue, can_push_into_not_full) {
    TQueue<int> queue(3);
    int n = 5431;
    queue.push(n);
    int n1 = queue.top();
    EXPECT_EQ(n, n1);
}

TEST(TQueue, cant_push_into_full) {
    TQueue<int> queue(1);
    queue.push(1);
    EXPECT_ANY_THROW(queue.push(2));
}

TEST(TQueue, can_pop_from_not_empty) {
    TQueue<int> queue(1);
    int n = 5431;
    queue.push(n);
    int n1 = queue.pop();
    EXPECT_EQ(n, n1);
}

TEST(TQueue, cant_pop_from_empty) {
    TQueue<int> queue(1);
    EXPECT_ANY_THROW(queue.pop());
}

TEST(TQueue, can_create_copied_TQueue) {
    TQueue<int> original(3);
    original.push(1);
    original.push(2);
    original.push(3);

    TQueue<int> copy(original);

    EXPECT_EQ(original.pop(), 1);
    EXPECT_EQ(copy.pop(), 1);
    EXPECT_EQ(original.pop(), 2);
    EXPECT_EQ(copy.pop(), 2);
    EXPECT_EQ(original.pop(), 3);
    EXPECT_EQ(copy.pop(), 3);
}

TEST(TQueue, can_copy_assign_TQueue) {
    TQueue<int> original(3);
    original.push(10);
    original.push(20);
    original.push(30);

    TQueue<int> copy(1);
    copy = original;

    EXPECT_EQ(original.get_size(), copy.get_size());

    EXPECT_EQ(original.pop(), copy.pop());
    EXPECT_EQ(original.pop(), copy.pop());
    EXPECT_EQ(original.pop(), copy.pop());
}

TEST(TQueue, can_create_moved_TQueue) {
    TQueue<int> original(3);
    original.push(1);
    original.push(2);
    original.push(3);

    TQueue<int> moved(std::move(original));

    EXPECT_EQ(original.get_size() + 1, 0);
    EXPECT_EQ(moved.get_size(), 3);

    EXPECT_EQ(moved.pop(), 1);
    EXPECT_EQ(moved.pop(), 2);
    EXPECT_EQ(moved.pop(), 3);
}

TEST(TQueue, can_move_assign_TQueue) {
    TQueue<int> original(3);
    original.push(10);
    original.push(20);
    original.push(30);

    TQueue<int> moved(1);
    moved = std::move(original);

    EXPECT_EQ(original.get_size() + 1, 0);
    EXPECT_EQ(moved.get_size(), 3);

    EXPECT_EQ(10, moved.pop());
    EXPECT_EQ(20, moved.pop());
    EXPECT_EQ(30, moved.pop());
}

TEST(TQueue, TQueue_from_vector) {
    std::vector<int> vec = { 1, 2, 3, 4, 5 };
    TQueue<int> q(vec);

    EXPECT_EQ(q.get_size(), 5);
    EXPECT_FALSE(q.isEmpty());

    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_EQ(q.pop(), vec[i]);
    }
}

TEST(TQueue, can_clear) {
    TQueue<int> q(3);
    q.push(10);
    q.push(20);

    EXPECT_FALSE(q.isEmpty());
    EXPECT_NO_THROW(q.pop());

    q.clear();

    EXPECT_TRUE(q.isEmpty());
    EXPECT_ANY_THROW(q.pop());
}

TEST(TQueue, to_vector_method) {
    TQueue<int> q(3);
    q.push(1);
    q.push(2);
    q.push(3);

    std::vector<int> vec = q.to_vector();

    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

TEST(TQueue, FIFO_behavior) {
    TQueue<int> q(3);
    q.push(1);
    q.push(2);
    q.push(3);

    EXPECT_EQ(q.pop(), 1);
    EXPECT_EQ(q.pop(), 2);
    EXPECT_EQ(q.pop(), 3);
}