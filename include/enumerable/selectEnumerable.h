#pragma once

#include <type_traits>

template <typename InType, typename Func, typename Source>
struct SelectEnumerable : EnumerableBase<
	typename std::result_of<Func(InType)>::type,
	SelectEnumerable < InType, Func, Source >
>
{
	typedef typename std::result_of<Func(InType)>::type RetType;
	SelectEnumerable(Source &source, Func &&func)
		: m_source(source)
		, m_func(func)
	{}


	virtual RetType value() override
	{
		return m_func(m_source.value());
	}

	virtual bool moveNext() override
	{
		return m_source.moveNext();
	}

private:
	Source m_source;
	Func m_func;
};
