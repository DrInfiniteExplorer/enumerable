#pragma once

template <typename T, typename SetType, typename Source>
struct DistinctEnumerable : InputRange<T, DistinctEnumerable<T, SetType, Source>>
{
	DistinctEnumerable(Source &source)
		: m_source(source)
	{}

	virtual T value() override
	{
		return m_source.value();
	}

	virtual bool moveNext() override
	{
		while (m_source.moveNext())
		{
			if (m_set.find(m_source.value()) == m_set.end())
			{
				m_set.insert(m_source.value());
				return true;
			}
		}
		return false;
	}

private:
	Source m_source;
	SetType m_set;
};

