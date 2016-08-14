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

int main(int argc, char* argv[])
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