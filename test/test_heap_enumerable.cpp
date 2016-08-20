#include <stdio.h>

#include <enumerable/enumerable.h>
#include <enumerable/inputIterator.h>
#include <enumerable/heapEnumerable.h>
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

struct Struct
{
	HeapEnumerable<float> m_enum;

};

Struct fromFunction(int (&arr)[7])
{
	return { Enumerable(arr).staticCast<float>().makeHeapEnumerable() };
}

int main(int argc, char* argv[])
{
	printf("starting\n");
	HeapEnumerable<int> g_heapEnum;
	try
	{
		int array[7] = { 0,1,2,3,4,5,6 };

		{
			auto Enum = Enumerable(array);

			g_heapEnum = Enum.makeHeapEnumerable();
		}

		//EnumerableBase<int, HeapEnumerable<int>>* ptr = &g_heapEnum;
		//HeapEnumerable<int>* derived = static_cast<HeapEnumerable<int>*>(ptr);

		Struct s;
		s.m_enum = g_heapEnum.staticCast<float>().makeHeapEnumerable();		

		g_heapEnum.forEach(printInt); printf("\n");
		s.m_enum.forEach(printFloat); printf("\n");

		auto&& ss = fromFunction(array);
		int asd;
		ss.m_enum.forEach(printFloat); printf("\n");
	}
	catch (std::exception& e)
	{
		printf("Exception caught: %s\n", e.what());
	}
	

	return 0;
}