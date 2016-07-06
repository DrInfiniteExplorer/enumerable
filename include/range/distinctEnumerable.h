#pragma once

template <typename T, typename SetType, typename Source>
struct DistinctEnumerable : InputRange<T, DistinctEnumerable<T, SetType, Source>>
{
	DistinctEnumerable(Source &source)
		: m_source(source)
	{}

	virtual bool empty() override
	{
		return m_source.empty();
	}

	virtual T front() override
	{
		return m_source.front();
	}

	virtual void popFront() override
	{
		m_source.popFront();
		if (m_source.empty()) return;
		while (m_set.find(m_source.front()) != m_set.end())
		{
			m_source.popFront();
			if (m_source.empty()) return;
		}
		m_set.insert(m_source.front());
	}

private:
	Source m_source;
	SetType m_set;
};

