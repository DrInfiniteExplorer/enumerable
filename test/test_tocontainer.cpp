#include <stdio.h>

#include <enumerable/enumerable.h>
#include <enumerable/inputIterator.h>
#include <vector>
#include <list>
#include <set>

#include <typeinfo>

#include <gtest/gtest.h>

void printInt(int i)
{
	printf("%d ", i);
}

void printFloat(float f)
{
	printf("%f ", f);
}

TEST(ToContainer, StdVectorExplicitType)
{
	int array[7] = { 0,1,2,3,4,5,6 };
	auto vector = Enumerable(array).toContainer<std::vector<int>>();
	ASSERT_EQ(7, vector.size());
	EXPECT_TRUE(std::equal(array, array+7, vector.begin()));
}

TEST(ToContainer, StdVectorImplicitType)
{
	int array[7] = { 0,1,2,3,4,5,6 };
	auto vector = Enumerable(array).toContainer<std::vector>();
	ASSERT_EQ(7, vector.size());
	EXPECT_TRUE(std::equal(array, array+7, vector.begin()));
}

TEST(ToContainer, StdListImplicitType)
{
	int array[7] = { 0,1,2,3,4,5,6 };
	auto list = Enumerable(array).toContainer<std::list>();
	ASSERT_EQ(7, list.size());
	EXPECT_TRUE(std::equal(array, array+7, list.begin()));
}

TEST(ToContainer, StdSetImplicitType)
{
	int array[7] = { 0,1,2,3,4,5,6 };
	auto list = Enumerable(array).toContainer<std::set>();
	ASSERT_EQ(7, list.size());
	// This depends on internal ordering of std::set, assumed to be ordered with "<"
	EXPECT_TRUE(std::equal(array, array+7, list.begin()));
}
