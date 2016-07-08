#include <stdio.h>

#include <range/range.h>
#include <range/inputRangeIterator.h>
#include <vector>
#include <list>
#include <set>

void printInt(int i)
{
	printf("%d ", i);
}

void printFloat(float f)
{
	printf("%f ", f);
}

int main(int argc, char* argv[])
{
	int array[7] = { 0,1,2,3,4,5,6 };
	std::vector<int> vector = { 0,1,2,3,4,5,6 };

	auto Enum = Enumerable(array);

	for (auto v : Enum.save())
	{
		printf("%d ", v);
	}
	printf("\n");
	
	for (auto v : Enum.select([](auto a) { return a * 2.2; }))
	{
		printf("%f ", v);
	}
	printf("\n");
	
	for (auto v : Enum.select([](auto a) {
		struct { int q; double b; } e;
		return e = { a, a * 2.2 };
	}))
	{
		printf("%d %f  ", v.q, v.b);
	}
	printf("\n");
	
	Enum.select([](auto a) {
		struct { int q; double b; } e;
		return e = { a, a * 2.2 };
	}).forEach([](auto v) { printf("%d %f  ", v.q, v.b); });
	printf("\n");
	
	vector = { 7,8,9,10,11,12 };
	Enumerable(array).forEach(printInt); printf(" & ");
	Enumerable(vector).forEach(printInt);
	printf(" concat: ");
	Enumerable(array).concat(Enumerable(vector)).forEach(printInt);
	printf("\n");
	
	auto floatVector = std::vector<float>{ 1.1f,2.2f,3.3f,4.4f };
	Enumerable(floatVector).forEach(printFloat);
	printf(" cast(int): ");
	Enumerable(floatVector).staticCast<int>().forEach(printInt);
	printf("\n");
	
	auto intList = std::list<int>{ 1,2,3,4,5 };
	printf("list<int> enumeration: "); Enumerable(intList).forEach(printInt);
	printf("\n");
	
	vector = { -1, 0, 1 };
	Enumerable(vector).forEach(printInt);
	printf(" All(>0): %s\n", Enumerable(vector).all([](auto x) { return x > 0; }) ? "Yes" : "No");
	
	vector = { 1, 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf(" All(>0): %s\n", Enumerable(vector).all([](auto x) { return x > 0; }) ? "Yes" : "No");
	
	vector = { -1, 0, 1 };
	Enumerable(vector).forEach(printInt);
	printf(" Any(<0): %s\n", Enumerable(vector).any([](auto x) { return x < 0; }) ? "Yes" : "No");
	
	vector = { 1, 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf(" Any(<0): %s\n", Enumerable(vector).any([](auto x) { return x < 0; }) ? "Yes" : "No");
	
	vector = { 1, 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf(" contains 2: %s\n", Enumerable(vector).contains(2) ? "Yes" : "No");
	
	vector = { 1, 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf(" contains 9: %s\n", Enumerable(vector).contains(9) ? "Yes" : "No");
	
	vector = { 1, 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf(" count: %d\n", Enumerable(vector).count());
	
	vector = { 1, 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf(" count(>=2): %d\n", Enumerable(vector).count([](auto x) { return x >= 2; }));
	
	vector = { 1, 2, 3 };
	printf(" DefaultIfEmpty(1,2,3): "); 	Enumerable(vector).defaultIfEmpty().forEach(printInt);
	printf("\n");
	
	vector = {};
	printf(" DefaultIfEmpty(): "); 	Enumerable(vector).defaultIfEmpty().forEach(printInt);
	printf("\n");
	
	vector = { 1, 2, 3 };
	printf(" ValueIfEmpty=5 (1,2,3): "); 	Enumerable(vector).valueIfEmpty(5).forEach(printInt);
	printf("\n");
	
	vector = {};
	printf(" ValueIfEmpty=5 (): "); 	Enumerable(vector).valueIfEmpty(5).forEach(printInt);
	printf("\n");
	
	vector = { 1, 2, 3, 4, 2, 3, 4, 5, 6 };
	Enumerable(vector).forEach(printInt);
	printf(" Distinct: ");
	Enumerable(vector).distinct().forEach(printInt);
	printf("\n");
	
	vector = { 1, 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf(" ElementAt(2): %d", Enumerable(vector).elementAt(2));
	printf("\n");
	
	vector = { 1, 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf(" ElementAtOrDefault(2): %d", Enumerable(vector).elementAtOrDefault(2));
	printf("\n");
	
	vector = { 1, 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf(" ElementAtOrDefault(5): %d", Enumerable(vector).elementAtOrDefault(5));
	printf("\n");
	
	vector = { 1, 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf(" ElementAtOrValue(2, 5): %d", Enumerable(vector).elementAtOrValue(2, 5));
	printf("\n");
	
	vector = { 1, 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf(" ElementAtOrValue(5, 5): %d", Enumerable(vector).elementAtOrValue(5, 5));
	printf("\n");
	
	vector = { 1, 2, 3 };
	Enumerable(array).forEach(printInt); printf(" | ");
	Enumerable(vector).forEach(printInt);
	printf(" A except B: "); Enumerable(array).except(Enumerable(vector)).forEach(printInt);
	printf("\n");
	
	vector = { 1, 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf("first: %d\n", Enumerable(vector).first());
	
	vector = { 1, 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf("first>2: %d\n", Enumerable(vector).first([](auto x) { return x > 2; }));
	
	vector = {2, 3};
	Enumerable(vector).forEach(printInt);
	printf("firstOrDefault: %d\n", Enumerable(vector).firstOrDefault());
	
	vector = {};
	Enumerable(vector).forEach(printInt);
	printf("firstOrDefault: %d\n", Enumerable(vector).firstOrDefault());
	
	vector = { 1, 5 };
	Enumerable(vector).forEach(printInt);
	printf("firstOrDefault(>2): %d\n", Enumerable(vector).firstOrDefault([](auto x) { return x > 2; }));
	
	vector = { };
	Enumerable(vector).forEach(printInt);
	printf("firstOrDefault(>2): %d\n", Enumerable(vector).firstOrDefault([](auto x) { return x > 2; }));
	
	vector = { -3, -1, 1, 3, 5, 7, 9};
	Enumerable(array).forEach(printInt); printf(" | ");
	Enumerable(vector).forEach(printInt);
	printf(" A intersect B: "); Enumerable(vector).intersect(Enumerable(array)).forEach(printInt);
	printf("\n");
	
	vector = { 1, 2, 3, 3, 3, 4, 5, 3, 6, 7, 8 };
	Enumerable(array).forEach(printInt); printf(" | ");
	Enumerable(vector).forEach(printInt);
	printf("Join A==B select A*B : ");
	Enumerable(array).join(Enumerable(vector),
		[](auto x) { return x; },
		[](auto x) { return x; },
		[](auto t1, auto t2) { return t1*t2; }).forEach(printInt);
	printf("\n");

	vector = { 1, 2, 3, 4, 5, 6, 7 };
	Enumerable(vector).forEach(printInt);
	printf(" where mod2=0: "); Enumerable(vector).where([](auto x) { return (x % 2) == 0; }).forEach(printInt);
	printf("\n");

	system("pause");

	return 0;
}