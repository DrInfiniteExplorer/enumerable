#pragma once

template <typename T, typename SetType, typename Source, typename OtherSource>
struct ExceptEnumerable : InputRange<T, ExceptEnumerable<T, SetType, Source, OtherSource>>
{
	ExceptEnumerable(Source &source, OtherSource&& otherSource)
		: m_source(source)
	{
		OtherSource other = otherSource;
		while (!other.empty())
		{
			m_set.insert(other.front());
			other.popFront();
		}
		while (!m_source.empty() && m_set.find(m_source.front()) != m_set.end())
		{
			m_source.popFront();
		}
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
		while (!m_source.empty() && m_set.find(m_source.front()) != m_set.end())
		{
			m_source.popFront();
		}
	}

private:
	Source m_source;
	SetType m_set;
};

