#include <stdio.h>

#include <enumerable/enumerable.h>
#include <enumerable/inputIterator.h>
#include <vector>
#include <list>
#include <set>

#include <typeinfo>

#include <gtest/gtest.h>

TEST(Iterator, RangedForEmpty)
{
	int array[1] = { 0 };
	int sum=0;
	for(auto v : Enumerable(array, 0))
	{
		sum += v;
	}
	EXPECT_EQ(0, sum);
}

TEST(Iterator, RangedForSum)
{
	int array[3] = { 1,2,3 };
	int sum=0;
	for(auto v : Enumerable(array))
	{
		sum += v;
	}
	EXPECT_EQ(6, sum);
}


TEST(Iterator, BeginNeqBegin)
{
	int array[2] = { 1,2 };
	auto Enum = Enumerable(array);
	EXPECT_NE(begin(Enum), begin(Enum));
}

TEST(Iterator, BeginNeqBegin2)
{
	int array[2] = { 1,2 };
	EXPECT_NE(begin(Enumerable(array)), begin(Enumerable(array)));
}

TEST(Iterator, BeginIsntEnd)
{
	int array[2] = { 1,2 };
	auto Enum = Enumerable(array);
	EXPECT_NE(begin(Enum), end(Enum));
}
	

int amain(int argc, char* argv[])
{
	int array[7] = { 0,1,2,3,4,5,6 };
	std::vector<int> vector = { 0,1,2,3,4,5,6 };

	auto Enum = Enumerable(array);

	auto start = begin(Enum);
	auto stop = end(Enum);
	if(start == stop) return 1;
	
	auto start2 = begin(Enum.save());
	if(start == start2) return 1;
	
	auto stop2 = end(Enum.save());
	if(stop == stop2) return 1;

	for (auto v : Enum)
	{
		printf("%d ", v);
	}

	return 0;
}