#include <stdio.h>

#include <gtest/gtest.h>

#include <enumerable/enumerable.h>

TEST(Enumerable, Fallthrough)
{
	int array[3] = { 1,2,3 };
	auto src = Enumerable(array);
	auto cpy = Enumerable(src);
	EXPECT_TRUE(src.sequenceEqual(cpy));	
}
