#pragma once

template <typename T>
struct InputRangeIterator
{
	InputRangeIterator(InputRange<T> &e, bool end)
		: m_e(e)
		, m_end(end)
	{
		if (!end) e.restart();
	}
	T operator*() const
	{
		return m_e.front();
	}
	bool operator==(const InputRangeIterator &other) const
	{
		if (&m_e != &other.m_e) return false;
		if (other.m_end && m_e.empty()) return true;
		return false;
	}

	bool operator!=(const InputRangeIterator &other) const
	{
		return !(*this == other);
	}


	void operator++()
	{
		m_e.popFront();
	}

private:
	InputRange<T> &m_e;
	bool m_end;
};

template <typename T>
InputRangeIterator<T> begin(InputRange<T> &a)
{
	return InputRangeIterator<T>(a, false);
}
template <typename T>
InputRangeIterator<T> end(InputRange<T> &a)
{
	return InputRangeIterator<T>(a, true);
}
