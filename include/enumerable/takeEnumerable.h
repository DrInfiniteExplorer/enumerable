#pragma once


template <typename T, typename Source>
struct TakeEnumerable : EnumerableBase<T, TakeEnumerable<T, Source>>
{
	TakeEnumerable(Source &source, int count)
		: m_source(source)
		, m_toTake(count)
		, m_taken(0)
	{}


	virtual T value() override
	{
		return m_source.value();
	}

	virtual bool moveNext() override
	{
		if(m_toTake < 1)
		{
			return false;
		}
		if(!m_source.moveNext())
		{
			return false;
		}
		++m_taken;
		if(m_taken > m_toTake)
		{
			return false;
		}
		return true;
	}

private:
	Source m_source;
	int m_toTake;
	int m_taken;
};
