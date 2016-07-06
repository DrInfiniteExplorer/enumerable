#pragma once

template<typename T>
struct ArrayEnumerable : public InputRange<T>
{
	ArrayEnumerable(T t[], size_t size)
		: m_t(t)
		, m_size(size)
		, m_index(0)
	{}

	virtual void restart() override
	{
		m_index = 0;
	}

	virtual bool empty() override
	{
		return m_index == m_size;
	}

	virtual T front()
	{
		return m_t[m_index];
	}

	virtual void popFront()
	{
		++m_index;
	}

private:
	T* m_t;
	size_t m_size;
	size_t m_index;
};

template<typename T, int size>
ArrayEnumerable<T> Enumerable(T(&t)[size])
{
	return ArrayEnumerable<T>(t, size);
}

