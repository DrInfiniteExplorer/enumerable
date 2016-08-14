#pragma once

// This class and respective begin/end function serve to create iterators to use
//  with standard algorithms and ranged-for statements in modern c++.
// The intent is that they are InputIterators. I do not know of a proper way to
//  test/verify this.

template <typename T, typename Source>
struct EnumerableBaseIterator
{
	static_assert(std::is_base_of<EnumerableBase<T, Source>, Source>::value, "Must inherit from EnumerableBase");
	typedef T value_type;

	EnumerableBaseIterator(Source &e, bool end)
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

	bool operator==(const EnumerableBaseIterator &other) const
	{
		if (&m_e != &other.m_e) return false;
		if (other.m_end && m_end) return true;
		return false;
	}

	bool operator!=(const EnumerableBaseIterator &other) const
	{
		return !(*this == other);
	}


	EnumerableBaseIterator& operator++()
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
EnumerableBaseIterator<T, Derived> begin(EnumerableBase<T, Derived> &a)
{
	Derived& derived = static_cast<Derived&>(a);
	return EnumerableBaseIterator<T, Derived>(derived, false);
}

template <typename T, typename Derived>
EnumerableBaseIterator<T, Derived> begin(EnumerableBase<T, Derived> &&a)
{
	Derived&& derived = static_cast<Derived&&>(a);
	return EnumerableBaseIterator<T, Derived>(derived, false);
}


template <typename T, typename Derived>
EnumerableBaseIterator<T, Derived> end(EnumerableBase<T, Derived> &a)
{
	Derived& derived = static_cast<Derived&>(a);
	return EnumerableBaseIterator<T, Derived>(derived, true);
}
template <typename T, typename Derived>
EnumerableBaseIterator<T, Derived> end(EnumerableBase<T, Derived> &&a)
{
	Derived&& derived = static_cast<Derived&&>(a);
	return EnumerableBaseIterator<T, Derived>(derived, true);
}
