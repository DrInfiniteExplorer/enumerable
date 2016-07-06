#pragma once

template <typename T, typename SetType>
struct DistinctEnumerable : InputRange<T>
{
	DistinctEnumerable(InputRange<T> &source)
		: m_source(source)
	{}

	virtual void restart() override
	{
		m_source.restart();
		m_set.clear();
		m_set.insert(m_source.front());
	}

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
	InputRange<T> &m_source;
	SetType m_set;
};

