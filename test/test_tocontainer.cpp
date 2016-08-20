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

	auto Enum = Enumerable(array);

	auto vector2 = Enum.save().toContainer<std::vector<int>>();
	Enumerable(vector2).forEach(printInt);

	auto vector = Enum.save().toContainer<std::vector>();
	Enumerable(vector).forEach(printInt);

	auto list = Enum.save().toContainer<std::list>();
	Enumerable(list).forEach(printInt);

	auto set = Enum.save().toContainer<std::set>();
	Enumerable(set).forEach(printInt);
	

	return 0;
}