#pragma once

template <typename InType, typename OutType, typename Source>
struct DynamicCastEnumerable : InputRange<OutType, DynamicCastEnumerable<InType, OutType, Source> >
{
	DynamicCastEnumerable(Source &source)
		: m_source(source)
	{}

	virtual void restart() override
	{
		m_source.restart();
	}

	virtual bool empty() override
	{
		return m_source.empty();
	}

	virtual OutType front() override
	{
		return dynamic_cast<OutType>(m_source.front());
	}

	virtual void popFront() override
	{
		m_source.popFront();
	}

private:
	Source m_source;
};

