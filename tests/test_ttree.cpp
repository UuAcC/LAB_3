#include <gtest/gtest.h>
#include "TTree.h"  

class TTreeTest : public ::testing::Test {
protected:
    TTree<int> tree;

    void SetUp() override {}

    void FillTree(const std::vector<int>& values) {
        for (int v : values) {
            tree.insert(v);
        }
    }
};

TEST_F(TTreeTest, InsertIntoEmptyTree) {
    tree.insert(10);
    EXPECT_TRUE(tree.contains(10));
    EXPECT_EQ(tree.min(), 10);
    EXPECT_EQ(tree.max(), 10);
}

TEST_F(TTreeTest, InsertMultipleElements) {
    FillTree({ 5, 3, 8, 1 });
    EXPECT_TRUE(tree.contains(5));
    EXPECT_TRUE(tree.contains(3));
    EXPECT_TRUE(tree.contains(8));
    EXPECT_TRUE(tree.contains(1));
    EXPECT_FALSE(tree.contains(10));
}

TEST_F(TTreeTest, InsertDuplicateThrows) {
    tree.insert(42);
    EXPECT_THROW(tree.insert(42), TreeExceptions);
}

TEST_F(TTreeTest, MinMaxAfterInsert) {
    FillTree({ 100, 50, 150, 30, 200 });
    EXPECT_EQ(tree.min(), 30);
    EXPECT_EQ(tree.max(), 200);
}

TEST_F(TTreeTest, MaxOnEmptyThrows) {
    EXPECT_THROW(tree.max(), TreeExceptions);
}

TEST_F(TTreeTest, MinOnEmptyThrows) {
    EXPECT_THROW(tree.min(), TreeExceptions);
}

TEST_F(TTreeTest, ContainsOnEmptyThrows) {
    EXPECT_THROW(tree.contains(5), TreeExceptions);
}

TEST_F(TTreeTest, RemoveLeaf) {
    FillTree({ 10, 5, 15 });
    tree.outsert(5);
    EXPECT_FALSE(tree.contains(5));
    EXPECT_TRUE(tree.contains(10));
    EXPECT_TRUE(tree.contains(15));
    EXPECT_EQ(tree.min(), 10);
}

TEST_F(TTreeTest, RemoveNodeWithLeftChild) {
    FillTree({ 20, 10, 5, 4 });

    tree.outsert(5);
    EXPECT_TRUE(tree.contains(10));
    EXPECT_FALSE(tree.contains(5));
    EXPECT_TRUE(tree.contains(20));
    EXPECT_TRUE(tree.contains(4));
}

TEST_F(TTreeTest, RemoveNodeWithRightChild) {
    FillTree({ 10, 20, 30, 40 });

    tree.outsert(30);
    EXPECT_TRUE(tree.contains(20));
    EXPECT_FALSE(tree.contains(30));
    EXPECT_TRUE(tree.contains(10));
    EXPECT_TRUE(tree.contains(40));
}

TEST_F(TTreeTest, RemoveNodeWithTwoChildren) {
    FillTree({ 50, 30, 70, 20, 40, 60, 80 });
    tree.outsert(50);
    EXPECT_FALSE(tree.contains(50));
    EXPECT_TRUE(tree.contains(30));
    EXPECT_TRUE(tree.contains(70));
    EXPECT_TRUE(tree.contains(20));
    EXPECT_TRUE(tree.contains(40));
    EXPECT_TRUE(tree.contains(60));
    EXPECT_TRUE(tree.contains(80));

    EXPECT_EQ(tree.getRootVal(), 40);
}

TEST_F(TTreeTest, RemoveNonExistentThrows) {
    tree.insert(1);
    tree.insert(2);
    EXPECT_THROW(tree.outsert(99), TreeExceptions);
}

TEST_F(TTreeTest, RemoveFromEmptyThrows) {
    EXPECT_THROW(tree.outsert(10), TreeExceptions);
}

TEST_F(TTreeTest, BalanceAfterLeftLeftInsertion) {
    FillTree({ 30, 20, 10 });
    EXPECT_EQ(tree.getRootVal(), 20);
}

TEST_F(TTreeTest, BalanceAfterRightRightInsertion) {
    FillTree({ 10, 20, 30 });
    EXPECT_EQ(tree.getRootVal(), 20);
}

TEST_F(TTreeTest, BalanceAfterLeftRightInsertion) {
    FillTree({ 30, 10, 20 });
    EXPECT_EQ(tree.getRootVal(), 20);
}

TEST_F(TTreeTest, BalanceAfterRightLeftInsertion) {
    FillTree({ 10, 30, 20 });
    EXPECT_EQ(tree.getRootVal(), 20);
}

TEST_F(TTreeTest, Balance1) {
    FillTree({ 10, 20, 30 });
    EXPECT_EQ(tree.getRootVal(), 20);

    FillTree({ 25, 45, 26 });
    EXPECT_EQ(tree.getRootVal(), 26);
}

TEST_F(TTreeTest, Balance2) {
    FillTree({ 50, 30, 80, 20, 40, 70, 90 });
    EXPECT_EQ(tree.getRootVal(), 50);

    FillTree({ 35, 38 });
    EXPECT_EQ(tree.getRootVal(), 50);
}

TEST_F(TTreeTest, Balance3) {
    FillTree({ 100, 60, 140, 40, 80, 120, 160, 30, 50, 70, 90 });
    EXPECT_EQ(tree.getRootVal(), 100);

    FillTree({ 45, 55 });
    EXPECT_EQ(tree.getRootVal(), 60);
}

TEST_F(TTreeTest, BalanceAfterRemovals) {
    FillTree({ 50, 30, 70, 20, 40, 60, 80, 35, 45, 65, 75, 85 });

    tree.outsert(30);
    tree.outsert(70);

    EXPECT_TRUE(tree.contains(50));
    EXPECT_TRUE(tree.contains(20));
    EXPECT_TRUE(tree.contains(40));
    EXPECT_TRUE(tree.contains(60));
    EXPECT_TRUE(tree.contains(80));
    EXPECT_TRUE(tree.contains(35));
    EXPECT_TRUE(tree.contains(45));
    EXPECT_TRUE(tree.contains(65));
    EXPECT_TRUE(tree.contains(75));
    EXPECT_TRUE(tree.contains(85));
    EXPECT_FALSE(tree.contains(30));
    EXPECT_FALSE(tree.contains(70));
}

TEST_F(TTreeTest, BalanceInsertAndRemoveChain) {
    FillTree({ 90, 50, 120, 30, 70, 100, 140, 20, 40, 60, 80 });
    EXPECT_EQ(tree.getRootVal(), 90);

    FillTree({ 65, 68 });
    EXPECT_EQ(tree.getRootVal(), 65);

    tree.outsert(90);
    tree.outsert(50);

    EXPECT_EQ(tree.min(), 20);
    EXPECT_EQ(tree.max(), 140);
    EXPECT_TRUE(tree.contains(68));
    EXPECT_TRUE(tree.contains(70));
    EXPECT_TRUE(tree.contains(65));
    EXPECT_FALSE(tree.contains(90));
    EXPECT_FALSE(tree.contains(50));
}

TEST_F(TTreeTest, InsertRemoveMinMax) {
    FillTree({ 5, 3, 8, 2, 4, 7, 9 });
    EXPECT_EQ(tree.min(), 2);
    EXPECT_EQ(tree.max(), 9);
    tree.outsert(2);
    EXPECT_EQ(tree.min(), 3);
    tree.outsert(9);
    EXPECT_EQ(tree.max(), 8);
    EXPECT_FALSE(tree.contains(2));
    EXPECT_FALSE(tree.contains(9));
}

TEST_F(TTreeTest, RemoveRootWithTwoChildren) {
    FillTree({ 100, 50, 150, 40, 60, 140, 160 });
    tree.outsert(100);
    EXPECT_FALSE(tree.contains(100));
    EXPECT_TRUE(tree.contains(50));
    EXPECT_TRUE(tree.contains(150));
    EXPECT_TRUE(tree.contains(40));
    EXPECT_TRUE(tree.contains(60));
    EXPECT_TRUE(tree.contains(140));
    EXPECT_TRUE(tree.contains(160));

    EXPECT_EQ(tree.min(), 40);
    EXPECT_EQ(tree.max(), 160);
}