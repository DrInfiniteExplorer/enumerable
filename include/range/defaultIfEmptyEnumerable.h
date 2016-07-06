#pragma once


template <typename T, typename Source>
struct DefaultIfEmptyEnumerable : InputRange<T, DefaultIfEmptyEnumerable<T, Source>>
{
	DefaultIfEmptyEnumerable(Source &source)
		: m_source(source)
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
			const auto& t = T();
			return t;
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
	bool m_wasEmpty;
	bool m_iterated;
};

