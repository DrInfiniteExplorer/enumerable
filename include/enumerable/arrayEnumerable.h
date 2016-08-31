#pragma once

#include <exception>
#include <stdexcept>

template <typename, typename>
struct EnumerableBase;

// This class wraps a normal array, providing a sequence object implementing EnumerableBase.
// The array is captured by pointer/reference, so sequences with this as the base can be
//  copied and reused multiple times without problem as long as the source array is valid.
template<typename T>
struct ArrayEnumerable : public EnumerableBase<T, ArrayEnumerable<T>>
{
	ArrayEnumerable(T t[], size_t size)
		: m_t(t)
		, m_size(size)
		, m_index(0)
		, m_firstMoved(false)
	{}

	virtual T value() override
	{
		if (!m_firstMoved)
		{
			throw std::runtime_error("ArrayEnumerable not moveNext'ed");
		}
		return m_t[m_index];
	}

	virtual bool moveNext() override
	{
		if (m_firstMoved && m_index >= m_size)
		{
			throw std::out_of_range("Tried to moveNext after end in ArrayEnumerable");
		}
		if (m_firstMoved)
		{
			++m_index;
		}
		m_firstMoved = true;
		return m_index < m_size;
	}

private:
	T* m_t;
	size_t m_size;
	size_t m_index;
	bool m_firstMoved;
};

// Create a sequence object given a fix-sized array
template<typename T, int size>
ArrayEnumerable<T> Enumerable(T(&t)[size])
{
	return ArrayEnumerable<T>(t, size);
}

// Create a sequence object given a pointer and a size
template<typename T>
ArrayEnumerable<T> Enumerable(T *t, size_t size)
{
	return ArrayEnumerable<T>(t, size);
}

