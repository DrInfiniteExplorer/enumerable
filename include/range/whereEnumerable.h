#pragma once

#include <type_traits>

template <typename T, typename Func, typename Source>
struct WhereEnumerable : InputRange<T, WhereEnumerable < T, Func, Source >>
{
	WhereEnumerable(Source &source, Func &&func)
		: m_source(source)
		, m_func(func)
	{
	}

	virtual T value() override
	{
		return m_source.value();
	}

	virtual bool moveNext() override
	{
		while (m_source.moveNext())
		{
			if (m_func(m_source.value()))
			{
				return true;
			}
		}
		return false;
	}

private:
	Source m_source;
	Func m_func;
};
