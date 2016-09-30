#include <stdio.h>

#include <gtest/gtest.h>

#include <enumerable/enumerable.h>
#include <enumerable/inputIterator.h>
#include <vector>
#include <list>
#include <set>

#include <typeinfo>

TEST(Skip, SkipNegative)
{
	int array[3] = { 1,2,3 };	
	EXPECT_EQ(3, Enumerable(array).skip(-1).count());
}

TEST(Skip, SkipZero)
{
	int array[3] = { 1,2,3 };	
	EXPECT_EQ(3, Enumerable(array).skip(0).count());
}

TEST(Skip, NotEverything)
{
	int array[3] = { 1,2,3 };	
	EXPECT_EQ(1, Enumerable(array).skip(2).count());
}

TEST(Skip, Everything)
{
	int array[3] = { 1,2,3 };	
	EXPECT_EQ(0, Enumerable(array).skip(20).count());
}

TEST(SkipWhile, Everything)
{
	int array[3] = { 1,2,3 };	
	EXPECT_EQ(0, Enumerable(array).skip([](auto x){ return true;}).count());
}

TEST(SkipWhile, Nothing)
{
	int array[3] = { 1,2,3 };	
	EXPECT_EQ(3, Enumerable(array).skip([](auto x){ return false;}).count());
}

TEST(SkipWhile, Some)
{
	int array[3] = { 1,2,3 };	
	EXPECT_EQ(1, Enumerable(array).skip([](auto x){ return x < 3;}).count());
}

TEST(SkipWhileIdx, Skip1)
{
	int array[3] = { 1,2,3 };	
	int count;
	EXPECT_EQ(2, Enumerable(array).skip([&](auto x, auto idx){ count = idx; return idx < 1;}).count());
	EXPECT_EQ(1, count);
}

TEST(SkipWhileIdx, Skip2)
{
	int array[3] = { 1,2,3 };	
	int count;
	EXPECT_EQ(1, Enumerable(array).skip([&](auto x, auto idx){ count = idx; return idx < 2;}).count());
	EXPECT_EQ(2, count);
}

TEST(SkipWhileIdx, Skip3)
{
	int array[3] = { 1,2,3 };
	int count;
	EXPECT_EQ(0, Enumerable(array).skip([&](auto x, auto idx){ count = idx; return idx < 3;}).count());
	EXPECT_EQ(2, count);
}

