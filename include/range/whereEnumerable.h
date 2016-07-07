#pragma once

#include <type_traits>

template <typename T, typename Func, typename Source>
struct WhereEnumerable : InputRange<T, WhereEnumerable < T, Func, Source >>
{
	WhereEnumerable(Source &source, Func &&func)
		: m_source(source)
		, m_func(func)
	{
		while (!m_source.empty() && !m_func(m_source.front()))
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
		while (!m_source.empty() && !m_func(m_source.front()))
		{
			m_source.popFront();
		}
	}

private:
	Source m_source;
	Func m_func;
};
