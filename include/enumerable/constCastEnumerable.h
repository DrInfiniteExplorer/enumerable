#pragma once

// A sequence object that performs a const-cast of elements in a source sequence.
template <typename InType, typename OutType, typename Source>
struct ConstCastEnumerable : EnumerableBase<OutType, ConstCastEnumerable<InType, OutType, Source>>
{
	ConstCastEnumerable(Source &source)
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
		return const_cast<OutType>(m_source.front());
	}

	virtual void popFront() override
	{
		m_source.popFront();
	}

private:
	Source m_source;
};

