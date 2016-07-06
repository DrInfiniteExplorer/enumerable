#pragma once

template <typename InType, typename Func>
struct SelectEnumerable : InputRange<typename std::result_of<Func(InType)>::type>
{
	typedef typename std::result_of<Func(InType)>::type RetType;
	SelectEnumerable(InputRange<InType> &source, Func &&func)
		: m_source(source)
		, m_func(func)
	{}

	virtual void restart() override
	{
		m_source.restart();
	}

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
	InputRange<InType> &m_source;
	Func& m_func;
};
