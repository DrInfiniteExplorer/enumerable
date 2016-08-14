#pragma once

// If the source sequence is not empty, this sequence object simply forwards that sequence.
// However, if the source sequence is empty, this object will contain a single default-initialized element.
template <typename T, typename Source>
struct DefaultIfEmptyEnumerable : EnumerableBase<T, DefaultIfEmptyEnumerable<T, Source>>
{
	DefaultIfEmptyEnumerable(Source &source)
		: m_source(source)
		, m_wasEmpty(false)
		, m_firstMoved(false)
	{}

	virtual T value() override
	{
		if (m_wasEmpty)
		{
			return T();
		}
		return m_source.value();
	}

	virtual bool moveNext() override
	{
		if (m_firstMoved && m_wasEmpty)
		{
			return false;
		}
		if (!m_firstMoved)
		{
			m_wasEmpty = !m_source.moveNext();
			m_firstMoved = true;
			return true;
		}
		return m_source.moveNext();
	}

private:
	Source m_source;
	bool m_wasEmpty;
	bool m_firstMoved;
};

