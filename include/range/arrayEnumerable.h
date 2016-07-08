#pragma once

template <typename, typename>
struct InputRange;

template<typename T>
struct ArrayEnumerable : public InputRange<T, ArrayEnumerable<T>>
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
		if (m_index == m_size)
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

template<typename T, int size>
ArrayEnumerable<T> Enumerable(T(&t)[size])
{
	return ArrayEnumerable<T>(t, size);
}

