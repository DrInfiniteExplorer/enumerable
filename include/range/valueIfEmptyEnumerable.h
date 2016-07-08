#pragma once

template <typename T, typename TT, typename Source>
struct ValueIfEmptyEnumerable : InputRange<T, ValueIfEmptyEnumerable<T, TT, Source>>
{
	ValueIfEmptyEnumerable(Source &source, TT&& value)
		: m_source(source)
		, m_value(value)
		, m_wasEmpty(false)
		, m_firstMoved(false)
	{}

	virtual T value() override
	{
		if (m_wasEmpty)
		{
			return m_value;
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
	TT m_value;
	bool m_wasEmpty;
	bool m_firstMoved;
};
