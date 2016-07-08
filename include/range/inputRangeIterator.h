#pragma once

template <typename T, typename Source>
struct InputRangeIterator
{
	InputRangeIterator(InputRange<T, Source> &e, bool end)
		: m_e(e)
		, m_end(end)
	{
		if (!m_end)
		{
			++(*this);
		}
	}
	T operator*() const
	{
		return m_e.value();
	}
	bool operator==(const InputRangeIterator &other) const
	{
		if (&m_e != &other.m_e) return false;
		if (other.m_end && m_end) return true;
		return false;
	}

	bool operator!=(const InputRangeIterator &other) const
	{
		return !(*this == other);
	}


	void operator++()
	{
		m_end = !m_e.moveNext();
	}

private:
	InputRange<T, Source> &m_e;
	bool m_end;
};

template <typename T, typename Source>
InputRangeIterator<T, Source> begin(InputRange<T, Source> &a)
{
	return InputRangeIterator<T, Source>(a, false);
}
template <typename T, typename Source>
InputRangeIterator<T, Source> end(InputRange<T, Source> &a)
{
	return InputRangeIterator<T, Source>(a, true);
}
