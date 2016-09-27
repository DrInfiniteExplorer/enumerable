#include <stdio.h>

#include <gtest/gtest.h>

#include <enumerable/enumerable.h>
#include <enumerable/inputIterator.h>
#include <vector>
#include <list>
#include <set>

#include <typeinfo>

TEST(Take, TakeNegative)
{
	int array[3] = { 1,2,3 };	
	EXPECT_EQ(0, Enumerable(array).take(-1).count());
}

TEST(Take, TakeZero)
{
	int array[3] = { 1,2,3 };	
	EXPECT_EQ(0, Enumerable(array).take(0).count());
}

TEST(Take, NotEverything)
{
	int array[3] = { 1,2,3 };	
	EXPECT_EQ(2, Enumerable(array).take(2).count());
}

TEST(Take, Everything)
{
	int array[3] = { 1,2,3 };	
	EXPECT_EQ(3, Enumerable(array).take(20).count());
}

TEST(TakeWhile, Everything)
{
	int array[3] = { 1,2,3 };	
	EXPECT_EQ(3, Enumerable(array).take([](auto x){ return true;}).count());
}

TEST(TakeWhile, Nothing)
{
	int array[3] = { 1,2,3 };	
	EXPECT_EQ(0, Enumerable(array).take([](auto x){ return false;}).count());
}

TEST(TakeWhile, Some)
{
	int array[3] = { 1,2,3 };	
	EXPECT_EQ(2, Enumerable(array).take([](auto x){ return x < 3;}).count());
}

TEST(TakeWhileIdx, Take1)
{
	int array[3] = { 1,2,3 };	
	int count;
	EXPECT_EQ(1, Enumerable(array).take([&](auto x, auto idx){ count = idx; return idx < 1;}).count());
	EXPECT_EQ(1, count);
}

TEST(TakeWhileIdx, Take2)
{
	int array[3] = { 1,2,3 };	
	int count;
	EXPECT_EQ(2, Enumerable(array).take([&](auto x, auto idx){ count = idx; return idx < 2;}).count());
	EXPECT_EQ(2, count);
}

TEST(TakeWhileIdx, Take3)
{
	int array[3] = { 1,2,3 };
	int count;
	EXPECT_EQ(3, Enumerable(array).take([&](auto x, auto idx){ count = idx; return idx < 3;}).count());
	EXPECT_EQ(2, count);
}

