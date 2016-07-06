#pragma once

template <typename T, typename TT, typename Source>
struct ValueIfEmptyEnumerable : InputRange<T, ValueIfEmptyEnumerable<T, TT, Source>>
{
	ValueIfEmptyEnumerable(Source &source, TT&& value)
		: m_source(source)
		, m_value(value)
		, m_wasEmpty(source.empty())
		, m_iterated(false)
	{}

	virtual bool empty() override
	{
		if (m_wasEmpty)
		{
			return m_iterated;
		}
		return m_source.empty();
	}

	virtual T front() override
	{
		if (m_wasEmpty)
		{
			return m_value;
		}
		return m_source.front();
	}

	virtual void popFront() override
	{
		if (m_wasEmpty)
		{
			m_iterated = true;
			return;
		}
		m_source.popFront();
	}

private:
	Source m_source;
	TT m_value;
	bool m_wasEmpty;
	bool m_iterated;
};
