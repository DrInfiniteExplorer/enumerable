#pragma once

#include <type_traits>

template <typename InType, typename Func, typename Source>
struct SelectEnumerable : InputRange<
	typename std::result_of<Func(InType)>::type,
	SelectEnumerable < InType, Func, Source >
>
{
	typedef typename std::result_of<Func(InType)>::type RetType;
	SelectEnumerable(Source &source, Func &&func)
		: m_source(source)
		, m_func(func)
	{}

	virtual bool empty() override
	{
		return m_source.empty();
	}

	virtual RetType front() override
	{
		return m_func(m_source.front());
	}

	virtual void popFront() override
	{
		m_source.popFront();
	}

private:
	Source m_source;
	Func m_func;
};
