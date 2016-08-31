#include <stdio.h>

#include <gtest/gtest.h>

#include <enumerable/enumerable.h>
#include <enumerable/inputIterator.h>
#include <vector>
#include <list>
#include <set>

#include <typeinfo>

TEST(ForEach, NormalLambda)
{
	int array[3] = { 1,2,3 };
	int sum = 0;
	Enumerable(array).forEach([&](auto x) { sum += x; });
	EXPECT_EQ(6, sum);
}
TEST(ForEach, RefArgLambda)
{
	int array[3] = { 1,2,3 };
	int sum = 0;
	Enumerable(array).forEach([&](auto x) { sum += x; });
	EXPECT_EQ(6, sum);
}

TEST(ForEach, ConstArgLambda)
{
	int array[3] = { 1,2,3 };
	int sum = 0;
	Enumerable(array).forEach([&](const auto x) { sum += x; });
	EXPECT_EQ(6, sum);
}

TEST(ForEach, ConstRefArgLambda)
{
	int array[3] = { 1,2,3 };
	int sum = 0;
	Enumerable(array).forEach([&](const auto& x) { sum += x; });
	EXPECT_EQ(6, sum);
}

TEST(ForEach, StoredNormalLambda)
{
	int array[3] = { 1,2,3 };
	int sum = 0;
	auto lambda = [&](auto x) { sum += x; };
	Enumerable(array).forEach(lambda);
	EXPECT_EQ(6, sum);
}
TEST(ForEach, StoredRefArgLambda)
{
	int array[3] = { 1,2,3 };
	int sum = 0;
	auto lambda = [&](auto x) { sum += x; };
	Enumerable(array).forEach(lambda);
	EXPECT_EQ(6, sum);
}

TEST(ForEach, StoredConstArgLambda)
{
	int array[3] = { 1,2,3 };
	int sum = 0;
	auto lambda = [&](const auto x) { sum += x; };
	Enumerable(array).forEach(lambda);
	EXPECT_EQ(6, sum);
}

TEST(ForEach, StoredConstRefArgLambda)
{
	int array[3] = { 1,2,3 };
	int sum = 0;
	auto lambda = [&](const auto& x) { sum += x; };
	Enumerable(array).forEach(lambda);
	EXPECT_EQ(6, sum);
}
