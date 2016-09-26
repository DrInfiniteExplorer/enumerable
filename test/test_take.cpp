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
