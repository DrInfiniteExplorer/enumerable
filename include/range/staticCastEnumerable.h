#pragma once

template <typename InType, typename OutType, typename Source>
struct StaticCastEnumerable : InputRange<OutType, StaticCastEnumerable<InType, OutType, Source>>
{
	StaticCastEnumerable(Source &source)
		: m_source(source)
	{}

	virtual bool empty() override
	{
		return m_source.empty();
	}

	virtual OutType front() override
	{
		return static_cast<OutType>(m_source.front());
	}

	virtual void popFront() override
	{
		m_source.popFront();
	}

private:
	Source m_source;
};

