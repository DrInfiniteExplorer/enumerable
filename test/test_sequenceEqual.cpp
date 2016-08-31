#include <stdio.h>

#include <gtest/gtest.h>

#include <enumerable/enumerable.h>
#include <enumerable/inputIterator.h>

TEST(SequenceEqual, TestEqual)
{
	int array[3] = { 1,2,3 };
	std::vector<int> vect = { 1,2,3 };
	EXPECT_TRUE(Enumerable(array).sequenceEqual(Enumerable(vect)));
}

TEST(SequenceEqual, TestFirstShort)
{
	int array[3] = { 1,2 };
	std::vector<int> vect = { 1,2,3 };
	EXPECT_FALSE(Enumerable(array).sequenceEqual(Enumerable(vect)));
}

TEST(SequenceEqual, TestSecondShort)
{
	int array[3] = { 1,2,3 };
	std::vector<int> vect = { 1,2 };
	EXPECT_FALSE(Enumerable(array).sequenceEqual(Enumerable(vect)));
}

TEST(SequenceEqual, TestEmpty)
{
	int array[1] = { 42 };
	std::vector<int> vect = { };
	EXPECT_TRUE(Enumerable(array,0).sequenceEqual(Enumerable(vect)));
}

TEST(SequenceEqual, TestFirstUnequal)
{
	int array[3] = { 0,2,3 };
	std::vector<int> vect = { 1,2,3 };
	EXPECT_FALSE(Enumerable(array).sequenceEqual(Enumerable(vect)));
}

TEST(SequenceEqual, TestLastUnequal)
{
	int array[3] = { 1,2,3 };
	std::vector<int> vect = { 1,2,4 };
	EXPECT_FALSE(Enumerable(array).sequenceEqual(Enumerable(vect)));
}

