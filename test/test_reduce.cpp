#include <stdio.h>

#include <enumerable/enumerable.h>
#include <vector>
#include <string>

#include <gtest/gtest.h>

TEST(Reduce, SeedFromParameter)
{
	int array[5] = { 1,2,3,4,5 };
	
	EXPECT_EQ(15, Enumerable(array).reduce([](int i, int sum) { return i + sum; },  0));
	EXPECT_EQ(25, Enumerable(array).reduce([](int i, int sum) { return i + sum; }, 10));
}

TEST(Reduce, SeedFromLambdaDefaultParameter)
{
	int array[5] = { 1,2,3,4,5 };

	EXPECT_EQ(15, Enumerable(array).reduce([](int i, int sum = 0) { return i + sum; }));
	EXPECT_EQ(25, Enumerable(array).reduce([](int i, int sum = 0) { return i + sum; }, 10));
}

TEST(Reduce, NoSeed)
{
	int array[5] = { 1,2,3,4,5 };

	EXPECT_EQ(15, Enumerable(array).reduce([](int i, int sum) { return i + sum; }));
}

TEST(Reduce, StringReduceTestLambda)
{
	std::vector<std::string> stringVector = {
		"The", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"
	};
	
	// Using normal seed
	auto reducedWhitespace = Enumerable(stringVector).reduce([](auto str, std::string sum) { return sum + " " + str; }, "");
	EXPECT_EQ(std::string(" The quick brown fox jumps over the lazy dog"), reducedWhitespace);

	// Using seed from lambda default
	auto reducedComma = Enumerable(stringVector).reduce([](std::string str, std::string sum="") { return sum + "," + str; });
	EXPECT_EQ(std::string(",The,quick,brown,fox,jumps,over,the,lazy,dog"), reducedComma);

	// First element is seed
	auto reducedWhitespaceNoSeed = Enumerable(stringVector).reduce([](auto str, std::string sum) { return sum + " " + str; });
	EXPECT_EQ(std::string("The quick brown fox jumps over the lazy dog"), reducedWhitespaceNoSeed);
}

std::string ReduceStringWhitespace(std::string str, std::string sum)
{
	return sum + " " + str;
}
std::string ReduceStringComma(std::string str, std::string sum = "")
{
	return sum + "," + str;
}

TEST(Reduce, StringReduceTestFixedFunc)
{
	std::vector<std::string> stringVector = {
		"The", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"
	};
	
	// Using normal seed
	auto reducedWhitespace = Enumerable(stringVector).reduce(ReduceStringWhitespace, "");
	EXPECT_EQ(std::string(" The quick brown fox jumps over the lazy dog"), reducedWhitespace);

	// Seed is used from first element, even though the function has a default argument.
	auto reducedComma = Enumerable(stringVector).reduce(ReduceStringComma);
	EXPECT_EQ(std::string("The,quick,brown,fox,jumps,over,the,lazy,dog"), reducedComma);

	// First element is seed
	auto reducedWhitespaceNoSeed = Enumerable(stringVector).reduce(ReduceStringWhitespace);
	EXPECT_EQ(std::string("The quick brown fox jumps over the lazy dog"), reducedWhitespaceNoSeed);
}

TEST(Reduce, EmptySequenceExceptionTest)
{
	std::vector<std::string> stringVector = {
	};
	
	// Using normal seed
	ASSERT_THROW(
		Enumerable(stringVector).reduce([](auto str, std::string sum) { return sum + " " + str; }, "")
		, std::out_of_range);

	// Using seed from lambda default
	ASSERT_THROW(
		Enumerable(stringVector).reduce([](std::string str, std::string sum="") { return sum + "," + str; })
		, std::out_of_range);

	// First element is seed
	ASSERT_THROW(
		Enumerable(stringVector).reduce([](auto str, std::string sum) { return sum + " " + str; })
		, std::out_of_range);

}

