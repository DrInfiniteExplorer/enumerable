#pragma once

template <typename T, typename Source>
struct InputRangeIterator
{
	static_assert(std::is_base_of<InputRange<T, Source>, Source>::value, "Must inherit rom InputRange");
	typedef T value_type;

	InputRangeIterator(Source &e, bool end)
		: m_e(e)
		, m_end(end)
		, m_firstMoved(false)
	{
	}
	T operator*()
	{
		if (!m_end && !m_firstMoved)
		{
			++(*this);
			m_firstMoved = true;
		}
		return m_e.value();
	}

	T operator->()
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


	InputRangeIterator& operator++()
	{
		m_end = !m_e.moveNext();
		return *this;
	}

private:
	Source& m_e;
	bool m_end;
	bool m_firstMoved;
};

template <typename T, typename Derived>
InputRangeIterator<T, Derived> begin(InputRange<T, Derived> &a)
{
	Derived& derived = static_cast<Derived&>(a);
	return InputRangeIterator<T, Derived>(derived, false);
}

template <typename T, typename Derived>
InputRangeIterator<T, Derived> begin(InputRange<T, Derived> &&a)
{
	Derived&& derived = static_cast<Derived&&>(a);
	return InputRangeIterator<T, Derived>(derived, false);
}


template <typename T, typename Derived>
InputRangeIterator<T, Derived> end(InputRange<T, Derived> &a)
{
	Derived& derived = static_cast<Derived&>(a);
	return InputRangeIterator<T, Derived>(derived, true);
}
template <typename T, typename Derived>
InputRangeIterator<T, Derived> end(InputRange<T, Derived> &&a)
{
	Derived&& derived = static_cast<Derived&&>(a);
	return InputRangeIterator<T, Derived>(derived, true);
}
