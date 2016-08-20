#include <stdio.h>

#include <enumerable/enumerable.h>
#include <enumerable/inputIterator.h>
#include <vector>
#include <list>
#include <set>

#include <typeinfo>

void printInt(int i)
{
	printf("%d ", i);
}

void printFloat(float f)
{
	printf("%f ", f);
}

struct Base { int x; Base(int x) : x(x) {} virtual ~Base() {} };
struct DerivedFloat : Base { float f; DerivedFloat(int a, float b) : Base{ a }, f(b) {} };
struct DerivedChar : Base { char c; DerivedChar(int a, char b) : Base{ a }, c(b) {} };
struct DerivedDouble : Base { double d; DerivedDouble(int a, double b) : Base{ a }, d(b) {} };

int main(int argc, char* argv[])
{
	int array[7] = { 0,1,2,3,4,5,6 };
	std::vector<int> vector = { 0,1,2,3,4,5,6 };

	auto Enum = Enumerable(array);

	printf("Foreach .save() ");
	for (auto v : Enum.save())
	{
		printf("%d ", v);
	}
	printf("\n");

	auto selected = Enum.select([](int a) { return a * 2.2; });
	for (auto v : selected)
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

	// GCC cant handle this. Tsk tsk tsk.
	//Enum.select([](auto a) {
	//	struct { int q; double b; } e;
	//	return e = { a, a * 2.2 };
	//}).forEach([](auto v) { printf("%d %f  ", v.q, v.b); });
	//printf("\n");

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

	vector = { 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf("firstOrDefault: %d\n", Enumerable(vector).firstOrDefault());

	vector = {};
	Enumerable(vector).forEach(printInt);
	printf("firstOrDefault: %d\n", Enumerable(vector).firstOrDefault());

	vector = { 1, 5 };
	Enumerable(vector).forEach(printInt);
	printf("firstOrDefault(>2): %d\n", Enumerable(vector).firstOrDefault([](auto x) { return x > 2; }));

	vector = {};
	Enumerable(vector).forEach(printInt);
	printf("firstOrDefault(>2): %d\n", Enumerable(vector).firstOrDefault([](auto x) { return x > 2; }));

	vector = { -3, -1, 1, 3, 5, 7, 9 };
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
		[](auto y) { return y; },
		[](auto t1, auto t2) { return t1*t2; }).forEach(printInt);
	printf("\n");

	struct Person
	{
		int id;
		std::string name;
	};

	struct House
	{
		int ownerId;
		std::string address;
	};
	Person persons[] = { {0, "Luben"}, {1, "Fungu"}, {2, "Plol"} };
	House houses[] = { { 0, "Slottet"}, {1, "Joenkoeping"}, {1, "Kallhaell"} };
	auto query = Enumerable(persons).join(Enumerable(houses),
		[](const auto& person) { return person.id; },
		[](const auto& house) { return house.ownerId; },
		[](const auto& person, const auto& house) 
		{
			struct Asd {
				std::string ownerName;
				std::string houseAddress;
			};
			return Asd{ person.name, house.address };
		}
	);
	auto filtered = query; // .where([](auto x) { return x.houseAddress == "Slottet"; });
	filtered.forEach([](auto x) { printf("%s owns a house at %s\n", x.ownerName.c_str(), x.houseAddress.c_str()); });

	vector = { 1, 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf("last: %d\n", Enumerable(vector).last());

	vector = { 1, 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf("last<3: %d\n", Enumerable(vector).last([](auto x) { return x < 3; }));

	vector = { 2, 3 };
	Enumerable(vector).forEach(printInt);
	printf("lastOrDefault: %d\n", Enumerable(vector).lastOrDefault());

	vector = {};
	Enumerable(vector).forEach(printInt);
	printf("lastOrDefault: %d\n", Enumerable(vector).lastOrDefault());

	vector = { 1, 5 };
	Enumerable(vector).forEach(printInt);
	printf("lastOrDefault(<3): %d\n", Enumerable(vector).lastOrDefault([](auto x) { return x < 3; }));

	vector = {};
	Enumerable(vector).forEach(printInt);
	printf("lastOrDefault(<3): %d\n", Enumerable(vector).lastOrDefault([](auto x) { return x < 3; }));

	vector = { 1, 5, 2, 9, 4, -2 };
	Enumerable(vector).forEach(printInt);
	printf("max(): %d\n", Enumerable(vector).max());

	vector = { 1, 5, 2, 9, 4, -2 };
	Enumerable(vector).forEach(printInt);
	printf("max(x*-1.5f): %f\n", Enumerable(vector).max([](auto x) { return x * -1.5f; }));

	vector = { 1, 5, 2, 9, 4, -2 };
	Enumerable(vector).forEach(printInt);
	printf("min(): %d\n", Enumerable(vector).min());

	vector = { 1, 5, 2, 9, 4, -2 };
	Enumerable(vector).forEach(printInt);
	printf("min(x*-1.5f): %f\n", Enumerable(vector).min([](auto x) { return x * -1.5f; }));

	Base* classPtrArray[] = {
		new Base{1},
		new DerivedFloat{2, 1.1f},
		new DerivedChar{3, 'a'},
		new DerivedDouble{4, 3.14}
	};
	Enumerable(classPtrArray).forEach([](auto x) { printf("%s ", typeid(*x).name()); });
	printf("ofType<DerivedFloat*>(): ");
	Enumerable(classPtrArray).ofType<DerivedFloat*>().select([](auto x) { return x->f; }).forEach(printFloat);
	printf("\n");

	vector = { 1, 2, 3, 4, 5, 6, 7 };
	Enumerable(vector).forEach(printInt);
	printf(" where mod2=0: "); Enumerable(vector).where([](auto x) { return (x % 2) == 0; }).forEach(printInt);
	printf("\n");

	system("pause");

	return 0;
}