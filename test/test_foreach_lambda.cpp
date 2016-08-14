#include <stdio.h>

#include <enumerable/enumerable.h>
#include <enumerable/inputIterator.h>
#include <vector>
#include <list>
#include <set>

#include <typeinfo>

int main(int argc, char* argv[])
{
	int array[3] = { 1,2,3 };

	printf("N ");
	Enumerable(array).forEach([](auto x) { printf("%d ", x); });
	printf("\n");

	printf("RR ");
	Enumerable(array).forEach([](auto&& x) { printf("%d ", x); });
	printf("\n");

	printf("C ");
	Enumerable(array).forEach([](const auto x) { printf("%d ", x); });
	printf("\n");

	printf("CR ");
	Enumerable(array).forEach([](const auto& x) { printf("%d ", x); });
	printf("\n");

	printf("LN ");
	auto lambda1 = [](auto x) { printf("%d ", x); };
	Enumerable(array).forEach(lambda1);
	printf("\n");

	printf("LRR ");
	auto lambda2 = [](auto&& x) { printf("%d ", x); };
	Enumerable(array).forEach(lambda2);
	printf("\n");

	printf("LC ");
	auto lambda3 = [](const auto x) { printf("%d ", x); };
	Enumerable(array).forEach(lambda2);
	printf("\n");

	printf("LCR ");
	auto lambda4 = [](const auto& x) { printf("%d ", x); };
	Enumerable(array).forEach(lambda4);
	printf("\n");

	const auto lambda5 = [](const auto& x) { printf("%d ", x); };
	Enumerable(array).forEach(lambda4);

	return 0;
}