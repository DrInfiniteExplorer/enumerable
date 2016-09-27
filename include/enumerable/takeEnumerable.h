#pragma once


template <typename T, typename Source, typename Predicate>
struct TakeEnumerable : EnumerableBase<T, TakeEnumerable<T, Source, Predicate>>
{
	TakeEnumerable(Source &source, Predicate pred)
		: m_source(source)
		, m_predicate(pred)
		, m_index(0)
	{}


	virtual T value() override
	{
		return m_source.value();
	}

	template <typename P = Predicate>
	bool doMoveNext(typename std::enable_if<!std::is_convertible<P, std::function<bool(T, size_t)>>::value,int>::type = 0 )
	{
		if(!m_source.moveNext())
		{
			return false;
		}
		if(!m_predicate(value()))
		{
			return false;
		}
		return true;
	}

	template <typename P = Predicate>
	bool doMoveNext(typename std::enable_if<std::is_convertible<P, std::function<bool(T, size_t)>>::value,int>::type = 0 )
	{
		if(!m_source.moveNext())
		{
			return false;
		}
		if(!m_predicate(value(), m_index++))
		{
			return false;
		}
		return true;
	}

	virtual bool moveNext() override
	{
		return doMoveNext();
	}
	
private:
	Source m_source;
	Predicate m_predicate;
	size_t m_index;
};

template <typename T, typename Source>
struct TakeEnumerable<T, Source, void> : EnumerableBase<T, TakeEnumerable<T, Source, void>>
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
