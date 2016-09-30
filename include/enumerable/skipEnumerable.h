#pragma once


template <typename T, typename Source, typename Predicate>
struct SkipEnumerable : EnumerableBase<T, SkipEnumerable<T, Source, Predicate>>
{
	SkipEnumerable(Source &source, Predicate pred)
		: m_source(source)
		, m_predicate(pred)
		, m_index(0)
		, m_skipped(false)
	{}


	virtual T value() override
	{
		return m_source.value();
	}

	template <typename P = Predicate>
	bool doMoveNext(typename std::enable_if<!std::is_convertible<P, std::function<bool(T, size_t)>>::value,int>::type = 0 )
	{
		while(m_source.moveNext())
		{
			if(!m_predicate(value())) return true;
		}
		return false;
	}

	template <typename P = Predicate>
	bool doMoveNext(typename std::enable_if<std::is_convertible<P, std::function<bool(T, size_t)>>::value,int>::type = 0 )
	{
		while(m_source.moveNext())
		{
			if(!m_predicate(value(), m_index++)) return true;
		}
		return false;
	}

	virtual bool moveNext() override
	{
		if(m_skipped)
		{
			return m_source.moveNext();
		}
		m_skipped = true;
		return doMoveNext();
	}
	
private:
	Source m_source;
	Predicate m_predicate;
	size_t m_index;
	bool m_skipped;
};

template <typename T, typename Source>
struct SkipEnumerable<T, Source, void> : EnumerableBase<T, SkipEnumerable<T, Source, void>>
{
	SkipEnumerable(Source &source, int count)
		: m_source(source)
		, m_toSkip(count)
		, m_skipped(0)
	{}


	virtual T value() override
	{
		return m_source.value();
	}

	virtual bool moveNext() override
	{		
		while(m_skipped < m_toSkip)
		{
			if(!m_source.moveNext())
			{
				return false;
			}
			m_skipped++;
		}
		return m_source.moveNext();
	}

private:
	Source m_source;
	int m_toSkip;
	int m_skipped;
};
